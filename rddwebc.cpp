#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <curl/curl.h>
#include <boost/filesystem.hpp>
#include <cstdlib>
#include <unistd.h>




/*
    Copyright (C) 2019, Rafael Godoy, <0x67rafael@protonmail.com> .

    RDDWEBC are true Open Source/Free Software and meet all definitions as such. It means that you are free to modify and
    redistribute all contents of the curl distributed archives.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>


    ______________________________________________________________________

    Curl and libcurl are true Open Source/Free Software and meet all definitions as such. It means that you are free to modify and
    redistribute all contents of the curl distributed archives.
    You may also freely use curl and libcurl in your commercial projects.

    Curl and libcurl are licensed under the license below, which is inspired by MIT/X, but not identical.

    Copyright (C) 1998 - 2015, Daniel Stenberg, <daniel@haxx.se>, et al.

    This software is licensed as described in the file COPYING, which
    you should have received as part of this distribution. The terms
    are also available at https://curl.haxx.se/docs/copyright.html.

    You may opt to use, copy, modify, merge, publish, distribute and/or sell
    copies of the Software, and permit persons to whom the Software is
    furnished to do so, under the terms of the COPYING file.

    This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
    KIND, either express or implied.
*/

using namespace boost;



static void banner_rddwebc(void){

    std::cout << "\033[1;31m\n\t========= <RDDWEBC WEB CRAWLING> [FOCUSED ON DIRECTORIES]  =========\n"
		           "\033[0;37m\n\tCopyright (C) 2019, Rafael Godoy, <0x67rafael@protonmail.com>\033[0m\n\n";

    std::cout << std::endl;

}


static void features_rddwebc(void){

    std::cout << "[*] FEATURES RDDWEBC:\n"
                 "\t\t\t BRUTEFORCING SEARCH USING WORDLIST\n"
                 "\t\t\t MANUAL SEARCH\n"
                 "\t\t\t USER-AGENT CHOICE\n" << std::endl;

}


void write_log(std::string data_log){

    std::string home_user = std::getenv("HOME");
    home_user.append("/.rddwebc_logs/");
    filesystem::create_directory(home_user);
    home_user.append("rddwebc_logs.txt");

    std::ofstream w_log;

    w_log.open(home_user, std::ios::app);

    if(!w_log.is_open()){
        std::cerr << "[!] CANNOT WRITE TO LOG FILE.." << std::endl;
        return;
    }

    data_log.append("\n");
    w_log << data_log;
    data_log.clear();
    w_log.close();
}




std::list<std::string> bforce_wordlist(const std::string &mode, const std::string &path){


    if(mode.compare("0") == 0){

        std::list<std::string> _path;
        _path.push_back(path);
        return _path;

    }else{

        std::ifstream path_reading;
        path_reading.open(path);

        if(!path_reading.is_open()){
            std::cerr << "[!] FAILURE TO OPEN WORDLIST: " << strerror(errno) << std::endl;
            _Exit(1);
        }

        std::list<std::string> w_list;
        std::string tmp_buf;

        while(std::getline(path_reading, tmp_buf))
            w_list.push_back(tmp_buf);

        tmp_buf.clear();
        path_reading.close();

        return w_list;
    }

}


size_t non_verbose(void *buffer, size_t size, size_t nmemb, void *userp){return size * nmemb;}



long rddwebc_get(const std::string &url, const std::string &user_agent){

    CURL *rddwebc;
    CURLcode response_code;
    long http_responde_code;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    rddwebc = curl_easy_init();
    if(rddwebc){
        curl_easy_setopt(rddwebc, CURLOPT_USERAGENT, user_agent.c_str());
        curl_easy_setopt(rddwebc, CURLOPT_URL, url.c_str());
        curl_easy_setopt(rddwebc, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(rddwebc, CURLOPT_WRITEFUNCTION, non_verbose);



#ifdef SKIP_PEER_VERIFICATION

    /*
     * If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */

        curl_easy_setopt(rddwebc, CURLOPT_SSL_VERIFYPEER, 0L);

#endif



#ifdef SKIP_HOSTNAME_VERIFICATION

    /*
     * If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */

        curl_easy_setopt(rddwebc, CURLOPT_SSL_VERIFYHOST, 0L);

#endif

        response_code = curl_easy_perform(rddwebc);

        if(response_code != CURLE_OK){
            std::cerr << "[!] RDDWEBC ERROR: " << curl_easy_strerror(response_code) << std::endl;
            curl_easy_cleanup(rddwebc);
            curl_global_cleanup();
            _Exit(1);
        }


        curl_easy_getinfo(rddwebc, CURLINFO_RESPONSE_CODE, &http_responde_code);


        curl_easy_cleanup(rddwebc);
    }

    curl_global_cleanup();

    return http_responde_code;

}




int main(void){

    try{

        banner_rddwebc();
        features_rddwebc();

        sleep(2);

        std::string url, mode, stringorwordlist, agent;

        do{
            std::cout << "[?] INSERT URL FOR CRAWLING: ";
            std::getline(std::cin, url);

        }while(url.empty() || (url.compare(" ") == 0));

        do{
            std::cout << "[?] BRUTEFORCE OR MANUAL TESTING [BRUTEFORCE = 1 || MANUAL = 0]: ";
            std::getline(std::cin, mode);

        }while(mode.empty() || (mode.compare(" ") == 0) || ((mode.compare("1") != 0) && (mode.compare("0") != 0)));

        if(mode.compare("0") == 0){
            do{
                std::cout << "[?] INSERT STRING FOR MANUAL TESTING:  ";
                std::getline(std::cin, stringorwordlist);

            }while(stringorwordlist.empty() || (stringorwordlist.compare(" ") == 0));

        }else if(mode.compare("1") == 0){
            do{
                std::cout << "[?] INSERT WORDLIST FOR BRUTEFORCE:  ";
                std::getline(std::cin, stringorwordlist);

            }while(stringorwordlist.empty() || (stringorwordlist.compare(" ") == 0));

        }

        std::cout << "[?] INSERT USER-AGENT FOR CRAWLING: ";
        std::getline(std::cin, agent);

        if(agent.empty() || agent.compare(" ") == 0){
            agent = "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)";
            std::cout << "[!] USING USER-AGENT STANDARD MODE" << std::endl;
        }





        std::list<std::string> _ulist = (bforce_wordlist(mode, stringorwordlist));

        std::cout << "\n\n[*] INITIATING THE REQUISITIONS..\n\n";

        for(std::list<std::string>::iterator _ulist_it = _ulist.begin(); _ulist_it != _ulist.end(); _ulist_it++){

                std::string url_search = url;

                url_search.append(("/" + *_ulist_it));

                long rddwebc_get_return = rddwebc_get(url_search, agent);

                std::string found_msg = "[***] URL REQUEST: " + url_search;

                found_msg.append(("\t\t\t\[CODE " + (std::to_string(rddwebc_get_return)) + "]"));

                std::cout << found_msg << "\n\n";


                write_log(found_msg);
                std::cout << "[*] PLEASE CHECK THE LOG FILE ($HOME/.rddwebc_logs/rddwebc_logs.txt)" << std::endl;

        }


    }catch(std::exception &fatal_error){
        std::cerr << "[!!!!] ERROR: " << fatal_error.what() << std::endl;
    }


    return 0;
}










