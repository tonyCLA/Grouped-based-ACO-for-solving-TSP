/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   output.h
 * All credit of this class goes to the authors of: http://stackoverflow.com/questions/13665090/trying-to-write-stdout-and-file-at-the-same-time
 * Accessed on 4th of May 2017
 * Class implementation which overloads << operator only for file write
 *   to write both on console and file at the same time 
 * minor change to append text instead of re-write a file
 * Created on 04 May 2017, 13:45
 */

#ifndef OUTPUT_H
#define OUTPUT_H

struct FileOutput : std::ofstream
{
    FileOutput(const std::string& fileName)
       : std::ofstream(fileName.c_str(), std::ios_base::app)
       , fileName(fileName.c_str())
    {};

    const std::string fileName;
};

template <typename T>
FileOutput& operator<<(FileOutput& strm, const T& var)
{
    std::cout << var;
    static_cast<std::ofstream&>(strm) << var;
    return strm;
};

#endif /* OUTPUT_H */

