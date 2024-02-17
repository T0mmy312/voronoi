#pragma once

#ifndef _DESMOS_H_
#define _DESMOS_H_

#include <fstream>
#include <string>

#define FILESTART "<body style=\"background-color:#2A2A2A;\" marginwidth=\"0px\" marginheight=\"0px\">\n<script src=\"https://www.desmos.com/api/v1.7/calculator.js?apiKey=dcb31709b452b1cf9dc26972add0fda6\"></script>\n<div id=\"calculator\"></div>\n<script>\n	var elt = document.getElementById(\"calculator\");\n	var calculator = Desmos.GraphingCalculator(elt);\n"

#define EXPR_START "\tcalculator.setExpression({\n       latex: '"
#define EXPR_MIDDLE "',\n       id: '"
#define EXPR_HIDDEN "',\n       hidden: "
#define EXPR_END ",\n  });\n"

class DesFile
{
private:
    int index;

public:
    DesFile(const char filename[]) {
        file = std::ofstream("perlinHashTest.txt");
        file << FILESTART;
        index = 0;
    }
    ~DesFile() {
        file << "</script>";
        file.close();
    }

    std::ofstream file;

    void addExpr(std::string latex, bool hidden = false) {
        file << EXPR_START << latex << EXPR_MIDDLE << index << EXPR_HIDDEN << (hidden ? "true" : "false") << EXPR_END;
        index++;
    }
};

#endif