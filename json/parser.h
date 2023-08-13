#pragma once

#include "json.h"

namespace starfarming {
    namespace json {

        class Parser
        {
        public:
            Parser();
            ~Parser();

            void load(const string& str);
            void load(const char* buf, int len);
            Json parse();

        private:
            void skip_white_space();
            char get_next_token();

            Json parse_null();
            Json parse_bool();
            Json parse_number();
            string parse_string();
            Json parse_array();
            Json parse_object();

            bool in_range(int x, int lower, int upper)
            {
                return (x >= lower && x <= upper);
            }

        private:
            string m_str;
            size_t m_idx;
        };

    }
}
