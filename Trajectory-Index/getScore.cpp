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
#include "convoy.hpp"

void getScore()
{
    std::string result = "";
    for (int i = 0; i < 45; i++) {
        std::string filePath = "/Users/dolan/workspace/GitClone/RExDBSCAN/query/whyNotResult" + Util::intToString(i) + ".txt";
        std::vector<std::string> queryVector = Util::readFileToVector(filePath.c_str());
        std::vector<std::string>::iterator it = queryVector.begin();
        for (; it != queryVector.end(); it ++) {
            std::vector<std::string> tempVector = Util::split(*it, "|");
            std::vector<std::string> idVector = Util::split(tempVector[0], ";");
            std::vector<std::string> queryVector1 = Util::split(tempVector[1], ";");
            std::string typeStr = "";
            int type = Util::stringToInt(idVector[0]);
            if (type == 0) {
                typeStr = "_e";
            }
            else if (type == 1) {
                typeStr = "_m";
            }
            else if (type == 2) {
                typeStr = "_me" + idVector[1];
            }
            else if (type == 3) {
                typeStr = "_k";
            }
            std::string path1 = ResultLocal"" + queryVector1[2] + "/" + queryVector1[3] + "/convoyResult.txt";
            std::string path2 = ResultLocal"" + queryVector1[2] + "/" + queryVector1[3] + "/convoyResult" + typeStr + ".txt";
            std::string path3 = ResultLocal"" + queryVector1[2] + "/" + queryVector1[3] + "/convoyResult_interval" + typeStr + ".txt";
            ConvoyManage* convoyManage1 = ConvoyManage::create(path1, "");
            ConvoyManage* convoyManage2 = ConvoyManage::create(path2, "");
            ConvoyManage* convoyManage3 = ConvoyManage::create(path3, "");
            int score1 = 10000, score2 = 10000;
            if (Util::stringToInt(queryVector1[6]) < 2) {
                
            }
            else
            {
                score1 = Util::compareScore(convoyManage1, convoyManage2);
                score2 = Util::compareScore(convoyManage1, convoyManage3);
            }
            
            std::cout<<score2<<"|"<<score1<<std::endl;
            result += queryVector1[2] + ";" + queryVector1[3] + ";" + idVector[0] + "|" + Util::intToString(score2) + ";" + Util::intToString(score1) + "\n";
            
            delete convoyManage1;
            delete convoyManage2;
            delete convoyManage3;
        }
        std::string path = "whyNotResult" + Util::intToString(i);
        printf("finish %s \n", path.c_str());
    }
    
    std::ofstream fout(ResultLocal"scoreResult.txt");
    fout<<result;
}

void getWhyNot()
{
    int j = 0;
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
//            resultTypeVector.push_back(Util::getMinM());
//            std::vector<ResultType> temp = Util::getMinME();
//            resultTypeVector.insert(resultTypeVector.end(),temp.begin(),temp.end());
//            resultTypeVector.push_back(Util::getMinK(tarTrjVector));
            
            //            Util::writeFile(FileLocal"" + DataSetStr[i] + "/" + outArgv[0] + "whyNotResult.txt", resultTypeVector);
            Util::writeFileQuery(FileLocal"whyNotResult" + Util::intToString(j++) + ".txt", DataSetStr[i], outArgv[0], resultTypeVector);
            
            printf("finish min data set <%s:%s> \n", DataSetStr[i].c_str(), outArgv[0].c_str());
        }
        
        DBSCANWHYNOT->purgeInstance();
        printf("finish data set ++++%s \n", DataSetStr[i].c_str());
    }
}
