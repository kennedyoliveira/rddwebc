#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>



// For readability //

using std::string;
using std::to_string;
using std::getenv;
using std::getline;
using std::fstream;
using std::ios_base;
using std::list;
using std::cin;
using std::iterator;
using capture_exception = std::exception;

using namespace boost;
using namespace boost::algorithm;

typedef long LONGINT;




/*
    Copyright (C) 2019, Rafael Godoy, <0x67rafael@protonmail.com> .

    RDDWEBC are true Open Source/Free Software and meet all definitions as such. It means that you are free to modify and
    redistribute all contents of the RDDWEBC distributed archives.

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






static void banner_rddwebc(void){

    printf("\033[1;31m\n\t========= <RDDWEBC WEB CRAWLING> [FOCUSED ON DIRECTORIES]  =========\n"
		           "\033[0;37m\n\t"
		           "Copyright (C) 2019, Rafael Godoy, <0x67rafael@protonmail.com>\033[0m\n\n");

}


static void features_rddwebc(void){


    printf("[*] FEATURES RDDWEBC:\n"
                 "\t\t\t BRUTEFORCING SEARCH USING WORDLIST\n"
                 "\t\t\t MANUAL SEARCH\n"
                 "\t\t\t USER-AGENT CHOICE\n\n\n");

}


static string write_log(string data_log){

    string home_user = getenv("HOME");

    home_user.append("/.rddwebc_logs/");

    if(!filesystem::create_directory(home_user)){
        // fprintf(stderr, "[!] CANNOT CREATE LOGS DIRECTORY\n");
        return "FUNC_BAD";
    }


    home_user.append("rddwebc_logs.txt");

    fstream w_log;

    w_log.open(home_user, ios_base::app);

    if(!w_log.is_open()){
        // fprintf(stderr, "[!] CANNOT WRITE TO LOG FILE..\n");
        return "FUNC_BAD";
    }

    data_log.append("\n");
    w_log << data_log;
    data_log.clear();
    w_log.close();

    return "FUNC_OK";
}




static list<string> bforce_wordlist(const string &mode, const string &path){


    if(mode.compare("0") == 0){
        list<string> _path;
        _path.push_back(path);
        return _path;

    }else{
        fstream path_reading;
        path_reading.open(path, ios_base::in);

        if(!path_reading.is_open()){
            fprintf(stderr, "[!] FAILURE TO OPEN WORDLIST: %s\n",
                     strerror(errno));
            _Exit(1);
        }

        list<string> w_list;
        string tmp_buf;

        while(getline(path_reading, tmp_buf))
            w_list.push_back(tmp_buf);

        tmp_buf.clear();
        path_reading.close();

        return w_list;
    }

}


size_t non_verbose(void *buffer, size_t size,
                   size_t nmemb, void *userp)
                   {return size * nmemb;}



static LONGINT rddwebc_get(const string &url, const string &user_agent){

    CURL *rddwebc;
    CURLcode response_code;
    LONGINT http_responde_code;

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
            string curl_error = (curl_easy_strerror(response_code));
            algorithm::to_upper(curl_error);
            fprintf(stderr, "[!] RDDWEBC ERROR: %s\n", curl_error.c_str());
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

        string url, mode, stringorwordlist, agent;

        do{
            printf("[?] INSERT URL FOR CRAWLING: ");
            getline(cin, url);
            algorithm::trim(url);

        }while(url.empty());

        do{
            printf("[?] BRUTEFORCE OR MANUAL TESTING [BRUTEFORCE = 1 || MANUAL = 0]: ");
            getline(cin, mode);
            algorithm::trim(mode);

        }while(mode.empty() ||
               ((mode.compare("1") != 0) && (mode.compare("0") != 0)));

        if(mode.compare("0") == 0){
            do{
                printf("[?] INSERT STRING FOR MANUAL TESTING: ");
                getline(cin, stringorwordlist);
                algorithm::trim(stringorwordlist);

            }while(stringorwordlist.empty());

        }else if(mode.compare("1") == 0){
            do{
                printf("[?] INSERT WORDLIST FOR BRUTEFORCE: ");
                getline(cin, stringorwordlist);
                algorithm::trim(stringorwordlist);

            }while(stringorwordlist.empty());

        }

        printf("[?] INSERT USER-AGENT FOR CRAWLING: ");
        getline(cin, agent);
        algorithm::trim(agent);

        if(agent.empty()){
            agent = "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)";
            printf("[!] USING USER-AGENT STANDARD MODE : %s\n", agent.c_str());
        }





        list<string> _ulist = bforce_wordlist(mode, stringorwordlist);

        printf("\n\n[*] INITIATING THE REQUISITIONS..\n\n");

        for(list<string>::iterator _ulist_it = _ulist.begin(); _ulist_it != _ulist.end(); _ulist_it++){

                string url_search = url;

                url_search.append(("/" + *_ulist_it));

                LONGINT rddwebc_get_return = rddwebc_get(url_search, agent);

                string found_msg = "[***] URL REQUEST: " + url_search;

                found_msg.append(("\t\t\t\[CODE " + (to_string(rddwebc_get_return)) + "]"));

                printf("%s\n\n", found_msg.c_str());


                if(write_log(found_msg).compare("FUNC_OK") == 0)
                   printf("[*] PLEASE CHECK THE LOG FILE ($HOME/.rddwebc_logs/rddwebc_logs.txt)\n");
                else
                    fprintf(stderr, "[!] FAILED CREATING LOGS\n");

        }


    }catch(capture_exception &fatal_error){
        fprintf(stderr, "[!!!!] ERROR: %s\n", fatal_error.what());
    }


    return 0;
}










