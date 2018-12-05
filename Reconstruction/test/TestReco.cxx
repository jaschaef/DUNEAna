#include <iostream>
#include <string>

#include "EcalHit.h"
#include "EcalCluster.h"
#include "EcalFunctions.h"

#include "TString.h"
#include "TFile.h"
#include "TString.h"


using namespace std;


int main(int argc, char* argv[]) {

	// if(argc!=2) {
	//    cerr << "Configuration file missing!" << endl
	//         << "Usage " << argv[0] << " <config.xml>" << endl;
	//    return 1;
	// }

	TString geoFile = "ND_Strawman_Concept_v01.gdml";
	bool saveGeoManager = false;

	//EcalHit static values
	EcalHit::useOriginalPositions = true;
	EcalHit::energyThreshold = 0.;
	EcalHit::transformInput = true;

	//EcalCluster static values
	EcalCluster::energyThreshold = 0.;
	EcalCluster::AntiKtRadius = 0.05;



	if(argc > 0)
		geoFile = TString(argv[1]);


	cout << " >>>>> Running the EcalTest <<<<<" << endl << endl;

	TGeoManager *geom = 0;
	TFile* infile = 0;

	if(geoFile.Contains("gdml"))
		geom = TGeoManager::Import(geoFile);
	else{
		infile = TFile::Open(geoFile);
		geom = (TGeoManager*) infile->Get("FAIRGeom");
	}

	if(!geom){
		cout << endl << "Problem loading geometry!" << endl;
		return 1;
	}

	cout << "Done loading gdml file." << endl << endl;

	if(saveGeoManager){
		vector<TString> splits = EcalFunctions::Split(geoFile, "/");
		TString fname = EcalFunctions::Split(splits[splits.size()-1], ".")[0]+".root";

		cout << " Saving GeoFile as '" << fname << "'." << endl;

		TFile* geomSaveFile = TFile::Open(fname, "RECREATE");

		geomSaveFile->cd();
		geom->Write();
		geomSaveFile->Write();
		geomSaveFile->Close();
	}

	EcalFunctions* func = new EcalFunctions();


	std::vector<EcalHit*> debugHits;
	EcalHit* debugHit = 0;

	// 1st cluster
	debugHit = new EcalHit(0.0021908278577, 0.00246217031963, -0.00265844399109, 167.0);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000265024515102, -5.87637585759e-18, -0.0303123388439, 167.872634888);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00164954771753, -0.0446678325534, -0.0138319209218, 168.199996948);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00161541090347, -0.376904428005, -0.0616362057626, 169.399993896);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00283323880285, -0.559919536114, -0.105224318802, 170.600006104);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000993694760837, 0.00782866962254, -0.00979296676815, 167.245544434);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(1.55933328472e-08, -2.7722363472, 1.85759115219, 183.800003052);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(3.37978817697e-05, -5.78088838665e-05, 6.59589641145e-05, 167.0);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00204520951957, 5.30608536597e-17, 3.91889188904e-05, 167.011199951);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00178633572068, 0.0036656667944, 0.00536474352702, 168.199996948);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00159760622773, -0.100269719958, 0.0412453077734, 169.399993896);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00239262240939, -0.167957082391, 0.156581625342, 170.600006104);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00170490855817, -0.268429160118, 0.369103938341, 171.800003052);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.0017010307638, -0.311274588108, 0.619041740894, 173.0);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00183264445513, -0.283360749483, 0.804549574852, 174.199996948);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00164135615341, 4.44089216468e-17, 1.04056477547, 174.682556152);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00165953219403, -0.273602843285, 0.423129469156, 172.049621582);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000310730159981, -0.140838190913, 0.111835196614, 170.215682983);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00147942849435, -0.516299962997, 4.7040977478, 193.573074341);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00144485582132, -2.79369497299, 1.87276625633, 183.923278809);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000510998885147, 0.49716603756, 0.847656130791, 174.752639771);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000510998885147, 0.49716603756, 0.847656130791, 174.752639771);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00146400823724, 15.4551963806, 15.1606588364, 194.600006104);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00188923813403, -0.680764734745, 4.4843621254, 211.399993896);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000204015421332, -0.377690374851, 4.05430030823, 212.399993896);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00192917347886, -0.322868943214, 4.0, 212.516098022);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00226974417455, -0.477629721165, 1.68700504303, 180.199996948);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000317558355164, 3.12181901932, 5.51062202454, 185.0);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00235179834999, -0.721983015537, 1.25074410439, 185.0);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00165492261294, -0.195437327027, 8.23979377747, 207.800003052);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00198875996284, -0.372764199972, 8.63164710999, 209.0);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000942551065236, 3.3954782486, 5.92555618286, 185.959335327);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000580455351155, -0.411724865437, 0.152906820178, 172.612960815);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00207025674172, -0.714616715908, 1.25345671177, 185.0);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00340964994393, -1.09662103653, 1.76658761501, 186.199996948);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000460672745248, -2.0, 2.86427974701, 187.091659546);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000510998885147, -1.31536066532, 1.19408988953, 185.447814941);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000510998885147, -1.31536066532, 1.19408988953, 185.447814941);
	debugHits.push_back(debugHit);


	// 2nd cluster
	// debugHit = new EcalHit(0.00176320655737, 14.1906003952, -2.43353247242e-06, 162.199996948);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00188267731573, 14.2970294952, 0.00228515593335, 163.399993896);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00157008704264, 14.4458999634, 0.0649199187756, 164.600006104);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00101889495272, 14.5936717987, 0.0497810170054, 165.800003052);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00149590813089, 14.6500797272, 2.77555760292e-18, 166.134780884);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00236679869704, 14.7910251617, -0.1171291098, 167.0);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00184468284715, 14.9751138687, -0.21157155931, 168.199996948);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00179976632353, 15.0951242447, -0.367394208908, 169.399993896);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00391181465238, 15.5435552597, -0.86385601759, 170.600006104);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00146268575918, 17.5698242188, -2.0, 171.184066772);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00115607539192, 18.2152862549, -2.49990749359, 171.600006104);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00203129695728, 18.4652061462, -2.0, 171.303192139);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00204883422703, 18.5558929443, -4.97945642471, 177.800003052);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00308686192147, 15.0250864029, -0.294474393129, 168.199996948);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00116629886907, 16.4473133087, -0.777321338654, 169.399993896);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00159040431026, 14.6813001633, 0.247619539499, 167.0);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00174588069785, 14.8472290039, 0.333509385586, 168.199996948);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00197048136033, 14.9785747528, 0.681685388088, 169.399993896);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00225690030493, 15.3383541107, 1.16889095306, 170.600006104);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00355353788473, 16.0062713623, 1.6973361969, 171.800003052);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00251726340503, 17.0923156738, 2.586820364, 173.0);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00111060228664, 17.9681110382, 3.21410918236, 174.0);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.000558144121896, 17.5627937317, 2.84550333023, 174.0);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(1.81993309525e-05, 17.7988529205, 3.21594405174, 173.98916626);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00183286971878, 14.6828136444, 0.239969462156, 167.0);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(1.55933328472e-08, 16.0334968567, -5.14361858368, 183.800003052);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.000613433599938, 16.8454933167, -8.0, 185.847961426);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.000392660062062, 16.8967227936, -8.17721843719, 185.97428894);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.000216763059143, 16.0278873444, -5.17160415649, 183.831787109);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.000510998885147, 17.6196708679, 2.76399850845, 173.999572754);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.000510998885147, 17.6196708679, 2.76399850845, 173.999572754);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00218025548384, 18.5728797913, -5.00091981888, 177.800003052);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00170071551111, 14.1904087067, -0.00075072637992, 162.199996948);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00207023718394, 14.2773313522, 0.203762307763, 163.399993896);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(1.46850497913e-05, 19.9968929291, 6.04518461227, 177.592880249);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.000729255320039, 20.0149917603, -6.0, 178.545806885);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.000519823282957, 20.2634048462, -6.13419532776, 178.439758301);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.000345350534189, 46.0, -14.5436410904, 189.518951416);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00153938634321, 46.111366272, -14.5555858612, 189.498001099);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.000753535656258, 38.1833190918, 6.98196220398, 199.399993896);
	// debugHits.push_back(debugHit);
	// debugHit = new EcalHit(0.00039651003317, 38.0863265991, 7.99897480011, 199.800796509);
	// debugHits.push_back(debugHit);



	vector<EcalHit*> hits = debugHits; //{eh, eh2};
	vector<EcalHit*> cells = EcalHit::AddCells(hits);
	cout << "Merged " << hits.size() << " hits to " << cells.size() << " cells." << endl;

	func->MakeThetaPhiMap("Cells_ThetaPhi", cells);


	vector<EcalCluster*> clusters = EcalCluster::GetClusters(cells);
	cout << "Merged " << cells.size() << " cells to " << clusters.size() << " clusters." << endl;

	func->MakeThetaPhiMap("Clusters_ThetaPhi", clusters);


	for(const auto& cluster : clusters){
	 	cluster->Print();
	}


	if(infile != 0){
		infile->Close();
		delete infile;
	}

	return 0;

}
