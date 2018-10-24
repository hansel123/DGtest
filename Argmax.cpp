#include "Argmax.h"
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <string>


int Argmax::mCount = 0;

Argmax::Argmax(const char* fileName, const std::string labelName, const std::string tagName) {
    mFile = fopen(fileName, "rb");
    if (!mFile) {
        std::cout << "Failed on loading filename: " << fileName << std::endl;
        exit(-1);
    }
    // else {
    //     std::cout << "File name: " << fileName << std::endl;
    // }
    
    mLabel.open(labelName, std::ios_base::in);
    if (!mLabel) {
        std::cout << "Failed on loading filename: " << labelName << std::endl;
        exit(-1);
    }
    // else {
    //     std::cout << "Label name: " << labelName << std::endl;
    // }

    mTag.open(tagName, std::ios_base::in);
    if (!mTag) {
        std::cout << "Failed on loading filename: " << tagName << std::endl;
        exit(-1);
    }
    // else {
    //     std::cout << "Tag name: " << tagName << std::endl;
    // }
}

Argmax::~Argmax() {
    fclose(mFile);
    mLabel.close();
    mTag.close();
}

void Argmax::setIndex (const std::vector<float> &vec) {
    auto itr = max_element(vec.begin(), vec.end());
    mIndex = distance(vec.begin(), itr);
}

int Argmax::getIndex () {
    return mIndex;
}

void Argmax::printResult(const std::vector<std::string> &label, const std::vector<std::string> &tag) {
    std::cout << "Tag name: " << tag.at(mCount) << std::endl;
    mCount++;
    std::cout << "Classified as: " << label.at(getIndex()) << std::endl << std::endl;
}

std::vector<std::string> Argmax::txtToVector(std::ifstream &textFile) {
    std::string str;
    std::vector<std::string> vec;
    while (getline(textFile,str)) {
        vec.push_back(str);
    }
    return vec;
}

void Argmax::run() {
    std::vector<std::string> label = txtToVector(mLabel);
    std::vector<std::string> tag = txtToVector(mTag);
    std::vector<float> vec;
    int classSize = label.size();
    int count = 0;
    float c;
    std::cout << std::endl << "Classification Result" << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl << std::endl;
    while (fread(&c, sizeof(float),1, mFile)) {
        vec.push_back(c);
        count++;
        if (count%classSize == 0) {
            setIndex(vec);
            printResult(label, tag);
            vec.clear();
         }
    }
}