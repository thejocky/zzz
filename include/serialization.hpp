#pragma once

#include "Meta.h"
#include "zzz_tree.hpp"

#include <concepts>
#include <numeric>
#include <string>
#include <vector>

#include <sstream> 

namespace zzz {

    template<typename T>
    T deserialize(Node* objRoot) {
        T object; 
        deserialize(object, objRoot);
        return object;
    }

    template <typename Class,
    typename = std::enable_if_t <meta::isRegistered<Class>()>>
    void deserialize(Class& object, Node* objRoot) {
        meta::doForAllMembers<Class>(
            [&object , &objRoot](auto& member) {
                auto node = objRoot->child(member.getName());
                if (node) {
                    using MemberT = meta::get_member_type<decltype(member)>;
                    if (member.hasSetter()) {
                        member.set(object, deserialize<MemberT>(node));
                    } else if (member.canGetRef()) {
                        deserialize(member.getRef(object), node);
                    } else {
                        // throw deserialize_error
                        //     ("Unable to deserialize read-only member");
                    }
                }
            }
        );
    }


    template<typename T>
    requires std::is_arithmetic_v<T>
    void deserialize(T& object, Node* objRoot) {
        std::stringstream ss;
        ss << objRoot->firstChild()->string();
        ss >> object;
    }

    void deserialize(bool& object, Node* objRoot) {
        object = (objRoot->firstChild()->string() == "true");
    }

    void deserialize(char*& object, Node* objRoot) {
        object = strcpy(new char[objRoot->firstChild()->string().length()+1],
            objRoot->firstChild()->string().c_str());
    }

    void deserialize(std::string& object, Node* objRoot) {
        object = objRoot->firstChild()->string();
    }

    template<typename T>
    void deserialize(std::vector<T>& object, Node* objRoot) {
        object.clear();
        object.resize(objRoot->childCount());
        auto iter = object.begin();
        for (Node* node = objRoot->firstChild(); node; node = node->sibling()) {
            deserialize(*iter, node);
            iter++;
        }
    }

} 