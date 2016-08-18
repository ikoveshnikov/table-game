/*
 * Copyright (c) 2016, Ivan Koveshnikov
 * ikoveshnik@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of ofp-pfe nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstddef>
#include <string>
#include <iostream>
#include <getopt.h>

#include "tg_types.h"
#include "file_ops.h"
#include "input.h"

void Usage (std::string program_name)
{
    size_t pos = program_name.find_last_of('/');
    if ((pos != std::string::npos) && (pos < program_name.length()))
    {
        program_name = program_name.substr(pos+1);
    }

    std::cout << "\n"
           "Usage: " << program_name << " OPTIONS\n"
           "\n"
           "OPTIONS:\n"
           "  -f, --file        File name containing input data\n"
           "  -h, --help              Display this help and exit.\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    optind = 1;
    static struct option longopts[] =
    {
        {"file",    required_argument, NULL, 'f'},
        {"help",       no_argument,       NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    bool parse_error = false;

    std::string filename;

    while (1)
    {
        int long_index = 0;
        int opt = getopt_long(argc, argv, "f:h:", longopts, &long_index);

        if (opt == -1)
            break;	/* No more options */

        switch (opt) {
        case 'f':
            filename = optarg;
            break;

        case 'h':
        default:
            parse_error = true;
            break;
        }
    }

    if (parse_error || filename.empty())
    {
        Usage(argv[0]);
        return 1;
    }

    optind = 1;		/* reset 'extern optind' from the getopt lib */

    FileInput fi (filename);
    InputData data (fi.GetData());

    if (InputData::Status::Ok != data.GetDataStatus())
    {
        std::cout << data.GetErrorString();
        return 1;
    }

    return 0;
}
