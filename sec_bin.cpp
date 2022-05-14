/* Security Types: Binary Source File */

#include "sec_bin.hpp"
#include "sec_hex.hpp"
#include "sec_b64.hpp"

#include <bitset>

namespace kim
{
    namespace sec
    {
        Binary::Binary() { }

        Binary::Binary(const std::string& p_str)
        {
            /* String length must be a multiple of 8 */
            if (p_str.length() % 8 != 0) {
                throw std::invalid_argument(std::string("The length of the string ")
                                            + p_str + std::string(" is not a multiple of 8"));             }

            m_bin.reserve(p_str.length() / 8);

            /* Check if the string is a valid binary string */
            for (std::size_t i{}; i < p_str.length(); i += 8) {
                for (uint8_t j{}; j < 8; j++) {
                    if ((p_str[i + j] != '0') && (p_str[i + j] != '1')) {
                        throw std::invalid_argument(p_str + std::string(" is not a valid Binary string"));
                    }
                }

                m_bin.push_back(static_cast<std::byte>(std::stoi(p_str.substr(i, 8), nullptr, 2)));
            }
        }

        Binary::~Binary() { }

        std::size_t Binary::length() const
        {
            return m_bin.size();
        }

        bool Binary::empty() const
        {
            return m_bin.empty();
        }

        void Binary::push_back(const std::byte& p_byte)
        {
            m_bin.push_back(p_byte);
        }

        void Binary::reserve(const std::vector<std::byte>::size_type p_size)
        {
            m_bin.reserve(p_size);
        }

        Binary& Binary::append(const std::string& p_str)
        {
            /* String length must be a multiple of 8 */
            if (p_str.length() % 8 != 0) {
                throw std::invalid_argument(std::string("The length of the string ")
                                            + p_str + std::string(" is not a multiple of 8"));             }

            /* Check if the string is a valid binary string */
            for (std::size_t i{}; i < p_str.length(); i += 8) {
                for (uint8_t j{}; j < 8; j++) {
                    if ((p_str[i + j] != '0') && (p_str[i + j] != '1')) {
                        throw std::invalid_argument(p_str + std::string(" is not a valid Binary string"));
                    }
                }

                m_bin.push_back(static_cast<std::byte>(std::stoi(p_str.substr(i, 8), nullptr, 2)));
            }

            return *this;
        }

        Hex Binary::to_Hex() const
        {
            Hex ret{};

            const char hex_table[] = { '0', '1', '2', '3',
                                       '4', '5', '6', '7',
                                       '8', '9', 'A', 'B',
                                       'C', 'D', 'E', 'F' };

            std::string tmp_str{};
            tmp_str.reserve(m_bin.size() * 2);

            for (std::vector<std::byte>::const_iterator it = m_bin.begin(); it != m_bin.end(); it++) {
                tmp_str.push_back(hex_table[std::to_integer<uint8_t>((*it & std::byte{0b11110000}) >> 4)]);
                tmp_str.push_back(hex_table[std::to_integer<uint8_t>(*it & std::byte{0b00001111})]);
            }

            ret.append(tmp_str);

            return ret;
        }

        Base64 Binary::to_B64() const
        {
            Base64 ret{};
            ret.reserve(m_bin.size() * 4 / 3 + (m_bin.size() * 4 % 3));

            const char base64_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G',
                                          'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                          'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                                          'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
                                          'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                          'j', 'k', 'l', 'm', 'n', 'o', 'p',
                                          'q', 'r', 's', 't', 'u', 'v', 'w',
                                          'x', 'y', 'z', '0', '1', '2', '3',
                                          '4', '5', '6', '7', '8', '9', '+', '/' };

            for (std::size_t i{}; i < (m_bin.size() / 3) * 3; i += 3) {
                std::string tmp_str{};

                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(m_bin[i] >> 2)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[i] & std::byte{0b00000011}) << 4)
                                             + std::to_integer<uint8_t>(m_bin[i + 1] >> 4)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>((m_bin[i + 1] & std::byte{0b00001111}) << 2)
                                             + std::to_integer<uint8_t>(m_bin[i + 2] >> 6)]);
                tmp_str.push_back(base64_table[std::to_integer<uint8_t>(m_bin[i + 2] & std::byte{0b00111111})]);

                ret.append(tmp_str);
            }

            return ret;
        }

        std::byte Binary::operator[](const std::size_t p_index) const
        {
            return m_bin[p_index];
        }

        std::byte& Binary::operator[](const std::size_t p_index)
        {
            return m_bin[p_index];
        }

        std::ostream& operator<<(std::ostream& os, const Binary& p_Bin)
        {
            for (auto& e : p_Bin.m_bin) {
                os << std::bitset<8>(std::to_integer<uint8_t>(e));
            }

            return os;
        }
    }
}
