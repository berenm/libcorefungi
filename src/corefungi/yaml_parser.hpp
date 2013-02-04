/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_YAML_PARSER_HPP__
#define __COREFUNGI_YAML_PARSER_HPP__

#include <boost/property_tree/node.hpp>
#include <boost/property_tree/detail/file_parser_error.hpp>

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace corefungi {

  namespace yaml {

    struct parser_error : boost::property_tree::file_parser_error {
      parser_error(std::string const& message, std::string const& file_name, size_t line) :
        file_parser_error(message, file_name, line)
      {}
    };

    corefungi::node read_helper(YAML::Node const& node) {
      if (node.Type() == YAML::NodeType::Map) {
        corefungi::dict v;
        for (YAML::Iterator child = node.begin(), end = node.end(); child != end; ++child) {
          v.emplace(corefungi::pair { child.first().to< std::string >(), read_helper(child.second()) });
        }

        return v;
      }

      if (node.Type() == YAML::NodeType::Sequence) {
        corefungi::list v;
        for (auto const& child : node) {
          v.emplace_back(read_helper(child));
        }

        return v;
      }

      return node.to< std::string >();
    } // read_helper

    void read_internal(std::istream& stream, corefungi::node& n, std::string const& file_name) {
      YAML::Parser parser(stream);
      YAML::Node   document;

      n = read_helper(YAML::Load(stream));
    }

    void read(std::istream& stream, corefungi::node& n) {
      read_internal(stream, n, "<stream>");
    }

    void read(std::string const& file_name, corefungi::node& n) {
      std::ifstream stream(file_name.c_str());

      if (!stream)
        throw parser_error("cannot open file", file_name, 0);

      read_internal(stream, n, file_name);
    }

    void write_helper(YAML::Emitter& stream, corefungi::node const& node) {
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

    void write_internal(std::ostream& stream, corefungi::node const& node, std::string const& file_name) {
      YAML::Emitter out;

      out << YAML::BeginDoc;
      write_helper(out, node);
      out << YAML::EndDoc;

      stream << out.c_str();
      if (!stream.good())
        throw parser_error("write error", file_name, 0);
    }

    void write(std::ostream& stream, corefungi::node const& node) {
      write_internal(stream, node, "<stream>");
    }

    void write(std::string const& file_name, corefungi::node const& node) {
      std::ofstream stream(file_name.c_str());

      if (!stream)
        throw parser_error("cannot open file", file_name, 0);

      write_internal(stream, node, file_name);
    }

  }
}

#endif // ifndef __COREFUNGI_YAML_PARSER_HPP__
