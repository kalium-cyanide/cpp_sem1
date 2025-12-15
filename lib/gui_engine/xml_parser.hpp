#include "dom_types.hpp"
#include <map>
#include <memory>

#include <utility>
#include <vector>

namespace xml_parser {

    std::unique_ptr<DomNode> parse(std::string_view in);

}// namespace xml_parser
