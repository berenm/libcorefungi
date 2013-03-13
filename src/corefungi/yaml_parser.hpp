/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_YAML_PARSER_HPP__
#define __COREFUNGI_YAML_PARSER_HPP__

#include "corefungi.hpp"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace corefungi {
  namespace cfg = ::corefungi;

  namespace yaml {

    cfg::node read_helper(YAML::Node const& node) {
      if (node.Type() == YAML::NodeType::Map) {
        cfg::dict v;
        for (YAML::const_iterator it = node.begin(), end = node.end(); it != end; ++it) {
          v.emplace(it->first.as< std::string >(), read_helper(it->second));
        }

        return v;
      }

      if (node.Type() == YAML::NodeType::Sequence) {
        cfg::list v;
        for (auto const& child : node) {
          v.emplace_back(read_helper(child));
        }

        return v;
      }

      if (node.Type() == YAML::NodeType::Scalar)
        return node.as< std::string >();

      return cfg::node {};
    } // read_helper

    void read_internal(std::istream& stream, cfg::node& n, std::string const& file_name) {
      n = read_helper(YAML::Load(stream));
    }

    void read(std::istream& stream, cfg::node& n) {
      read_internal(stream, n, "<stream>");
    }

    void read(std::string const& file_name, cfg::node& n) {
      std::ifstream stream(file_name.c_str());

      if (!stream)
        throw std::runtime_error("cannot open file: " + file_name);

      read_internal(stream, n, file_name);
    }

    void write_helper(YAML::Emitter& stream, cfg::node const& node) {
      if (is_a< spore >(node)) {
        spore const& v = boost::get< spore >(node);

#define YAML_TRY_GET(type_m)                     \
  do {                                           \
    try {                                        \
      stream << static_cast< type_m >(v);        \
      return;                                    \
    } catch (boost::bad_lexical_cast const& e) { \
    }                                            \
  } while (0)

        YAML_TRY_GET(bool);
        YAML_TRY_GET(int64_t);
        YAML_TRY_GET(uint64_t);
        YAML_TRY_GET(float);
        YAML_TRY_GET(double);

#undef YAML_TRY_GET

        stream << YAML::SingleQuoted << static_cast< std::string >(v);

      } else if (is_a< list >(node)) {
        list const& v = boost::get< list >(node);

        if (v.empty())
          stream << YAML::Flow;

        stream << YAML::BeginSeq;
        for (auto const& n : v) {
          write_helper(stream, n);
        }
        stream << YAML::EndSeq;

      } else if (is_a< dict >(node)) {
        dict const& v = boost::get< dict >(node);

        if (v.empty())
          stream << YAML::Flow;

        stream << YAML::BeginMap;
        for (auto const& p : v) {
          stream << YAML::Key << p.first;
          stream << YAML::Value;
          write_helper(stream, p.second);
        }
        stream << YAML::EndMap;

      }
    } // write_helper

    void write_internal(std::ostream& stream, cfg::node const& node, std::string const& file_name) {
      YAML::Emitter out;

      out << YAML::BeginDoc;
      write_helper(out, node);
      out << YAML::EndDoc;

      stream << out.c_str();
      if (!stream.good())
        throw std::runtime_error("write error: " + file_name);
    }

    void write(std::ostream& stream, cfg::node const& node) {
      write_internal(stream, node, "<stream>");
    }

    void write(std::string const& file_name, cfg::node const& node) {
      std::ofstream stream(file_name.c_str());

      if (!stream)
        throw std::runtime_error("cannot open file: " + file_name);

      write_internal(stream, node, file_name);
    }

  }
}

#endif // ifndef __COREFUNGI_YAML_PARSER_HPP__
