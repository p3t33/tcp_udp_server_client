#ifndef _DATA_PROXY_
#define _DATA_PROXY_
/*******************************************************************************
*                             data proxy
*                       =====================
* File Name: data_proxy.hpp
* Related files: data_proxy.cpp
* #Version: V 1.0
* Writer: Kobi Medrish       
* Created: 6.11.19
* Last update: 11.11.19
*******************************************************************************/


/*============================================================================*/
/*                                  Definitions                               */
/*============================================================================*/
/*                                                       standard directories */
/*                                                       ~~~~~~~~~~~~~~~~~~~~ */
#include <string> // td::string
#include <fstream> // std::ofstream
#include <vector> // std::vector

/*============================================================================*/


namespace med
{

class DataProxy
{
    public:
        DataProxy(std::string name_of_output_file = "server_output.txt");
        ~DataProxy();

        DataProxy(const DataProxy&) = delete;
        DataProxy& operator=(const DataProxy&) = delete;
        DataProxy(const DataProxy&&) = delete;
        DataProxy& operator=(const DataProxy&&) = delete;

        // Interface / API
        // ------------------------------------------------------------------
        void gate_way(const std::string& string);
        void write_to_file();

    private:
        // managing variables 
        // -----------------------------
        std::ofstream m_file; // write only mode
        std::vector<std::string> m_data;
        std::string m_string;
};

} // namespace med
#endif // _DATA_PROXY_
