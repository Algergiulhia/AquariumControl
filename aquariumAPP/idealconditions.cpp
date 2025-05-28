#include "idealconditions.h"

IdealConditions::IdealConditions()
{
    this->lightTime = 20*60 + 30;
    this->feedTime[0] = 43;
    this->feedTime[1] = 22*60 + 45;
    this->lightTimeDuration = 3;
    //this->tds = 400;

    calculateIdeal();
}

IdealConditions& IdealConditions::getInstance() {
    static  IdealConditions instance;
    return instance;
}

IdealConditions::~IdealConditions() { }

void IdealConditions::calculateIdeal() {
    mqd_t msq = createMsgQueue(mqNameW.c_str());
    std::string idealCondReq = "IDEAL";
    mq_send(msq,idealCondReq.c_str(), idealCondReq.size(), 0);
    mq_close(msq);
    char* readIdealCond = readMsgQueue(mqNameR.c_str());
    std::vector<std::string> idealParams = splitString(readIdealCond, '+');

    foodQuantity = std::stof(idealParams.at(0));
    temp = std::stof(idealParams.at(1));
    ph = std::stof(idealParams.at(2));

    return;
}

void IdealConditions::updtNewFishParam(std::vector<std::string> newparam) {
    fishParams[0] = std::stof(newparam.at(0));   // food - new fish
    fishParams[1] = std::stof(newparam.at(1));   // temp - new fish
    fishParams[2] = std::stof(newparam.at(2));   // ph   - new fish

    return;
}


void IdealConditions::setTDS(float tds) {
    this->tds = tds;
    return;
}

void IdealConditions::setTemp(float temp) {
    this->temp = temp;
    return;
}

void IdealConditions::setPh(float ph) {
    this->ph = ph;
    return;
}

float IdealConditions::getLightDuration() {
    return lightTimeDuration;
}

float IdealConditions::getTDS() {
    return tds;
}

float IdealConditions::getTemp() {
    return temp;
}

float IdealConditions::getPh() {
    return ph;
}

float IdealConditions::getLightTime() {
    return lightTime;
}

float IdealConditions::getFeedTime(bool x) {
    return feedTime[x];
}

float IdealConditions::getFood() {
    return foodQuantity;
}

void IdealConditions::setLightTime(float lightTimeH, float lightTimeM) {
    this->lightTime = ((lightTimeH*60)+lightTimeM);
}

void IdealConditions::setFeedTime(float feedTimeH, float feedTimeM, bool index) {
    this->feedTime[index] = ((feedTimeH*60)+feedTimeM);
}

void IdealConditions::setLightTimeDuration(float lightDurH, float lightDurM) {
    this->lightTimeDuration = ((lightDurH*60)+lightDurM);
}

bool IdealConditions::verifyAddition() {

    float cmpTEMP = fishParams[1] - temp;
    float cmpPH   = fishParams[2] - ph;

    cmpTEMP = (cmpTEMP < 0) ? -cmpTEMP : cmpTEMP;
    cmpPH = (cmpPH < 0) ? -cmpPH : cmpPH;

    if(cmpTEMP <= 2.01 && cmpPH <= 0.51)
        return false;
    else
        return true;
}
