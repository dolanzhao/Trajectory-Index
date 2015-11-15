//
//  main.cpp
//  Trajectory-Index
//
//  Created by Dolan on 15/11/7.
//  Copyright © 2015年 Dolan. All rights reserved.
//

#include <iostream>
#include "dbscan_whynot.hpp"
#include "util.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    
    for (int i = 0; i < (int)DataSetNumber; i++) {
        printf("start data set ++++%s \n", DataSetStr[i].c_str());
        std::string trjNodePath =  FileLocal"" + DataSetStr[i] + "/" + DataSetStr[i] + ".txt";
        DBSCANWHYNOT->initNode(trjNodePath);
        std::string argvPath =  FileLocal"" + DataSetStr[i] + "/argv.txt";
        std::vector<std::string> minSetArgv = Util::readFileToVector(argvPath);
        
        std::vector<std::string>::iterator it = minSetArgv.begin();
        for (; it != minSetArgv.end(); it++) {
            std::vector<std::string> outArgv = Util::split(*it, "|");
            std::string dbScanPath = FileLocal"" + DataSetStr[i] + "/" + outArgv[0] + "/clusterResult.txt";
            printf("start min data set <%s:%s> \n", DataSetStr[i].c_str(), outArgv[0].c_str());
            
            DBSCANWHYNOT->updateDbscanClass(dbScanPath, outArgv[1]);
            
            std::vector<int> tarTrjVector;
            tarTrjVector.push_back(DBSCANWHYNOT->_misId);
            std::vector<std::string> groupId = Util::split(outArgv[2], ";");
            std::string convoyPath = FileLocal"" + DataSetStr[i] + "/" + outArgv[0] + "/convoyResult.txt";
            std::map<std::string,std::string> tempMap;
            std::vector<std::string> convoyResult = Util::readFileToVector(convoyPath);
            std::vector<std::string>::iterator convoyIt = convoyResult.begin();
            std::vector<std::string>::iterator groupIdIt = groupId.begin();
            for (; convoyIt != convoyResult.end(); convoyIt++) {
                std::vector<std::string> tempString = Util::split(*convoyIt, "|");
                if (!(std::strcmp(tempString[0].c_str(), (*groupIdIt).c_str()))) {
                    std::vector<std::string> tempTarTrjVector = Util::split(tempString[1], ";");
                    std::vector<std::string>::iterator tempTarTrjVectorIt = tempTarTrjVector.begin();
                    for (; tempTarTrjVectorIt != tempTarTrjVector.end(); tempTarTrjVectorIt++) {
                        tarTrjVector.push_back(Util::stringToInt(*tempTarTrjVectorIt));
                    }
                }
                tempMap.insert(std::map<std::string,std::string>::value_type(tempString[0], Util::split(tempString[1], ";")[0] + ";" + tempString[2]));
            }
            for (; groupIdIt != groupId.end(); groupIdIt ++) {
                std::vector<std::string> tempString = Util::split(tempMap[*groupIdIt], ";");
                DBSCANWHYNOT->addTimeTargetSet(Util::stringToInt(tempString[1]), Util::stringToInt(tempString[2]), Util::stringToInt(tempString[0]));
            }
            std::vector<ResultType> resultTypeVector;
            resultTypeVector.push_back(Util::getMinE());
            resultTypeVector.push_back(Util::getMinM());
            std::vector<ResultType> temp = Util::getMinME();
            resultTypeVector.insert(resultTypeVector.end(),temp.begin(),temp.end());
            resultTypeVector.push_back(Util::getMinK(tarTrjVector));
            
            Util::writeFile(FileLocal"" + DataSetStr[i] + "/" + outArgv[0] + "/whyNotResult.txt", resultTypeVector);
            
            printf("finish min data set <%s:%s> \n", DataSetStr[i].c_str(), outArgv[0].c_str());
        }
        
        DBSCANWHYNOT->purgeInstance();
        printf("finish data set ++++%s \n", DataSetStr[i].c_str());
    }
    return 0;
}
