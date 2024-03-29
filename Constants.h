#pragma once
#define sqt3 sqrt(1.0/3.0)
#define sqt5 sqrt(3.0/5.0)
#define pi9 (5.0/9.0)
#define os9 (8.0/9.0)
#define cz_pl sqrt((3.0/7.0) + ((2.0/7.0)*sqrt(6.0/5.0)))
#define cz_mi sqrt((3.0/7.0) - ((2.0/7.0)*sqrt(6.0/5.0)))
#define wei_pl  (18.0+sqrt(30))/36
#define wei_min (18.0-sqrt(30))/36

//***********************************************************************
int SCHEMAT = 3;
int ILE_PKT;
//***********************************************************************

double gTot = -9;
double gCond = -9;
double gAlfa = -9;
int gElSize = -9;
int gNodSize = -9;
double gSpecHeat = -9;
double gDensity = -9;
double gSimStepTime = -9;
double gSimTime = -9;
double gInitTemp = -9;

//**************************Globalne, ustawiane w zale¿noœci od schematu ca³kowania
vector<vector<double>> GLOBAL_N_PO_ETA;
vector<vector<double>> GLOBAL_N_PO_KSI;

vector<vector<double>> GLOBAL_PKT_CALK;
vector<double> GLOBAL_WAGI_CALK;

vector<double> GLOBAL_WAGI_SCIANA;
vector<vector<vector<double>>> GLOBAL_PKT_SCIANA;
//Ksi = X, Eta = Y
//**************************Specyficzne punkty calkowania, etc
//Pkt Calkowania wewnatrzne

vector<vector<double>> PktCalk2D{
    {-sqt3,-sqt3},
    {sqt3,-sqt3},
    {-sqt3,sqt3},
    {sqt3,sqt3}
};
vector<vector<double>> PktCalk3D{
    {-sqt5,-sqt5},{0,-sqt5},{sqt5,-sqt5},
    {-sqt5,0},{0,0},{sqt5,0},
    {-sqt5,sqt5},{0,sqt5},{sqt5,sqt5}
};
vector<double> wagi3D{
    {pi9 * pi9},{os9 * pi9},{pi9 * pi9},
    {pi9 * os9},{os9 * os9},{pi9 * os9},
    {pi9 * pi9},{os9 * pi9},{pi9 * pi9}
};
vector<double> wagi2D{ 1,1,1,1 };

vector<vector<double>> PktCalk4D{
    //Dol ||Y=-0.86
    { -cz_pl,-cz_pl},
    {-cz_mi,-cz_pl},
    {cz_mi,-cz_pl},
    {cz_pl,-cz_pl},
    //mid-dol, Y=-0.33
    {-cz_pl,-cz_mi},
    {-cz_mi,-cz_mi},
    {cz_mi,-cz_mi},
    {cz_pl,-cz_mi},
    //mid-upper | Y=0.33
    {-cz_pl,cz_mi},
    {-cz_mi,cz_mi},
    {cz_mi,cz_mi},
    {cz_pl,cz_mi},
    //Gora | Y=0.86
    {-cz_pl,cz_pl},
    {-cz_mi,cz_pl},
    {cz_mi,cz_pl},
    {cz_pl,cz_pl}
};
vector<double> wagi4D{
    //Dolne
    {wei_min * wei_min},
    {wei_pl*wei_min},
    {wei_pl * wei_min},
    {wei_min * wei_min},
    //Mid-lower
    {wei_pl * wei_min},
    {wei_pl* wei_pl},
    {wei_pl* wei_pl},
    {wei_pl * wei_min},
    //Mid-Gorne
    {wei_pl * wei_min},
    {wei_pl* wei_pl},
    {wei_pl* wei_pl},
    {wei_pl * wei_min},
    //Gorne
    {wei_min * wei_min},
    {wei_pl * wei_min},
    {wei_pl * wei_min},
    {wei_min* wei_min}

};




//Pkt Calkowania na scianach
//Ksi = X, Eta = Y
vector<vector<double>> bottomPC_2D = {
    {-sqt3,-1.0},{sqt3,-1.0}
};
vector<vector<double>> rightPC_2D = {
    {1.0, -sqt3},{1.0, sqt3}
};
vector<vector<double>> leftPC_2D = {
    {-1.0, sqt3},{-1.0, -sqt3}
};
vector<vector<double>> topPC_2D = {
    {sqt3, 1.0},{-sqt3, 1.0 }
};
vector<vector<vector<double>>> PktCalkHBC_2D = { bottomPC_2D,   rightPC_2D,     topPC_2D,   leftPC_2D };
vector<double> weightsBC_2D =                   { 1,1,          1,1,              1,1,          1,1 };

//Ksi = X, Eta = Y
vector<vector<double>> bottomPC_3D = {
    {-sqt5,-1},{0,-1},{sqt5,-1}
};
vector<vector<double>> rightPC_3D = {
    {1,-sqt5},{1,0},{1,sqt5}
};
vector<vector<double>>topPC_3D = {
    {-sqt5,1},{0,1},{sqt5,1}
};
vector<vector<double>> leftPC_3D = {
    {-1,-sqt5},{-1,0},{-1,sqt5}
};
vector<vector<vector<double>>> PktCalkHBC_3D = { bottomPC_3D,   rightPC_3D,     topPC_3D,       leftPC_3D };
vector<double> weightsBC_3D =                  { pi9,os9,pi9,   pi9,os9,pi9,    pi9,os9,pi9,    pi9,os9,pi9 };
//Ksi = X, Eta = Y
vector<vector<double>> leftPC_4d = {
    {-1, cz_pl},
    {-1, cz_mi},
    {-1,-cz_mi},
    {-1,-cz_pl}
};
vector<vector<double>> bottomPC_4d = {
    {-cz_pl,-1},
    {-cz_mi,-1},
    {cz_mi,-1},
    {cz_pl,-1}
};
vector<vector<double>> rightPC_4d = {
    {1,-cz_pl},
    {1,-cz_mi},
    {1,cz_mi},
    {1,cz_pl}
};
vector<vector<double>> topPC_4D = {
    {cz_pl,1},
    {cz_mi,1},
    {-cz_mi,1},
    {-cz_pl,1}
};
vector<vector<vector<double>>> PKtcalkHBC_4D = { bottomPC_4d,rightPC_4d,topPC_4D,leftPC_4d };
vector<double> weightsBC_4D = { wei_min, wei_pl,wei_pl, wei_min,
                                wei_min, wei_pl,wei_pl, wei_min ,
                                wei_min, wei_pl,wei_pl, wei_min ,
                                wei_min, wei_pl,wei_pl, wei_min };

//*************************** Agregacja Danych
vector<vector<double>> agregatorH;
vector<vector<double>> agregatorC_2D;
vector<double> agregatorWektoraP;
vector<double> agregatorC_1D;
vector<double> agregatorH_1D;

//**************** SYMULACJA
vector<double> P_C;
vector<vector<double>> Saved_Temperatura;
vector<double> temperatures;