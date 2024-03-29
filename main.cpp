#include <iostream>
#include <vector>
#include "Node.h"
#include "Functions.h"
#include "Element.h"
#include "GlobalData.h"
#include "Grid.h"
#include "Constants.h"
#include "SurfaceOperator.h"
#include "Agregatory.h"

using namespace std;

//Ksi = X, Eta = Y

void algorithmSimulation();
string filepath = "FILEPATH_TO_WALL_FILE";
void setupBC(GlobalData& glob);

int main()
{
    SCHEMAT = 4;
    ILE_PKT = SCHEMAT * SCHEMAT;

    setupPointsWeightsCalk();
    setup_global_N();

    cout << "N_Eta:\n" << GLOBAL_N_PO_ETA << "\nN_Ksi:\n" << GLOBAL_N_PO_KSI;
    //filepath = "Test1_4_4.txt";
    GlobalData glob(filepath);

    //Correct the BC
    setupBC(glob);

	AgregatWektoraP agregatP_Agent;
	AgreagatMacierzyH agregatH_Agent;
	AgregatMacierzyC agregatC_Agent;

    //Pętla po wszystkich elementach
    for (Element x : glob.grid.elements) {
        x.setUp();

		x.printData();
        //x.printDataCalk();

		agregatP_Agent.ammendAgregacja(x);
		agregatH_Agent.ammendAgregacja(x);
		agregatC_Agent.ammendAgregacja(x);
    }
	agregatC_Agent.prepare1D();
	agregatH_Agent.prepare1D();

    cout << agregatorH;
    cout << "BC:" << agregatorWektoraP;


	//system("cls");
	algorithmSimulation();
    //system("cls");
    for (int i = 0; i < Saved_Temperatura.size(); i++)
        displayMinAndMax(Saved_Temperatura[i]);


    return 0;
}

void setupBC(GlobalData& glob) {
    for (Element el : glob.grid.elements)
        for (Node x : el.Nodes)
            for (int i : glob.grid.bc)
                if (x.id == i)
                    x.BC = 1;

}



