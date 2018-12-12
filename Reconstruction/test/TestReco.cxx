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

	TString geoFile = "../input/ND_Strawman_Concept_v01.root";
	bool saveGeoManager = false;

	//EcalHit static values
	EcalHit::useOriginalPositions = false;
	EcalHit::energyThreshold = 0.;
	EcalHit::transformInput = false;

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
		geom = (TGeoManager*) infile->Get("Default");
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
	vector<EcalHit*> cells;
	vector<EcalCluster*> clusters;

	bool doSimpleTest = false;

	if(!doSimpleTest){
		TGeoNode* currentNode = geom->GetTopNode();
		// cout << "curr = " << currentNode->GetName() << endl;

		TString nodeName = "BarrelECal_stave_module_layer_3_slice1_vol_0";
		TString baseName = "volNDHPgTPC_0/BarrelECal_vol_0/BarrelECal_stave_vol_0/BarrelECal_stave_module_layer_3_vol_0/";
		TGeoNode* node = func->GetNode(baseName+nodeName);
		EcalHit* hit = new EcalHit(10, node, "volWorld_1/"+baseName);
		hit->Print();
		cells.push_back(hit);

		nodeName = "BarrelECal_stave_module_layer_40_slice1_vol_0";
		baseName = "volNDHPgTPC_0/BarrelECal_vol_0/BarrelECal_stave_vol_0/BarrelECal_stave_module_layer_40_vol_0/";
		node = func->GetNode(baseName+nodeName);
		hit = new EcalHit(10, node, "volWorld_1/"+baseName);
		hit->Print();
		cells.push_back(hit);

		nodeName = "BarrelECal_stave_module_layer_40_slice2_vol_0";
		baseName = "volNDHPgTPC_0/BarrelECal_vol_0/BarrelECal_stave_vol_0/BarrelECal_stave_module_layer_40_vol_0/";
		node = func->GetNode(baseName+nodeName);
		hit = new EcalHit(10, node, "volWorld_1/"+baseName);
		hit->Print();
		cells.push_back(hit);


		nodeName = "EndcapECal_stave_module_layer_40_slice3_vol_0";
		baseName = "volNDHPgTPC_0/EndcapECal_vol_0/EcalEndcapStave_vol_0/EndcapECal_stave_module_layer_40_vol_0/";
		node = func->GetNode(baseName+nodeName);
		hit = new EcalHit(10, node, "volWorld_1/"+baseName);
		hit->Print();
		cells.push_back(hit);

		hit = new EcalHit(10, 316.34, 0, 0);
		hit->Print();
		cells.push_back(hit);


		clusters = EcalCluster::GetAntiKtClusters(cells);

		// EcalCluster* cluster = new EcalCluster();
		// for(const auto& h : cells)
		// 	cluster->AddHit(h);
		// cluster->ComputeDirection();
		// clusters.push_back(cluster);

	}else{


		std::vector<EcalHit*> debugHits_50;
		EcalHit* debugHit = 0;

		// 1st cluster - 50 MeV photon
		debugHit = new EcalHit(0.00108022685163, 0.0, 0.0, 162.726455688);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.00192801153753, -0.00128100172151, 0.0166151933372, 163.399993896);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.00193497422151, 0.0837961658835, 0.0804075524211, 164.600006104);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.00298676453531, 0.499042063951, -0.19498474896, 165.800003052);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.00208172108978, 0.91620349884, -0.941529154778, 167.0);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.00332155753858, 0.447562724352, -0.183049425483, 165.800003052);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.00197000149637, 0.0672629848123, 0.274432271719, 165.800003052);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.00157540373039, -0.0466206036508, 0.487023442984, 166.307022095);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(2.91009996545e-07, 0.559882164001, 0.576713204384, 177.800003052);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.000142001328641, 0.0, 0.0, 162.726455688);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.000116217866889, -5.10808314274e-17, -0.00720243295655, 162.825912476);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.000841665198095, -3.46944695195e-17, -0.00986553635448, 162.896408081);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.00153199280612, 0.89878821373, 0.920054912567, 182.600006104);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(6.76444833516e-05, 0.581693470478, 0.598888516426, 178.366699219);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.000358581339242, -1.87733447552, 2.8905134201, 176.600006104);
		debugHits_50.push_back(debugHit);
		debugHit = new EcalHit(0.00187426886987, -1.88883364201, 2.90827870369, 176.683914185);
		debugHits_50.push_back(debugHit);


		std::vector<EcalHit*> debugHits_150;
		// 2nd cluster  - 150 MeV Photon
		debugHit = new EcalHit(0.00176320655737, 14.1906003952, -2.43353247242e-06, 162.199996948);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00188267731573, 14.2970294952, 0.00228515593335, 163.399993896);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00157008704264, 14.4458999634, 0.0649199187756, 164.600006104);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00101889495272, 14.5936717987, 0.0497810170054, 165.800003052);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00149590813089, 14.6500797272, 2.77555760292e-18, 166.134780884);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00236679869704, 14.7910251617, -0.1171291098, 167.0);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00184468284715, 14.9751138687, -0.21157155931, 168.199996948);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00179976632353, 15.0951242447, -0.367394208908, 169.399993896);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00391181465238, 15.5435552597, -0.86385601759, 170.600006104);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00146268575918, 17.5698242188, -2.0, 171.184066772);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00115607539192, 18.2152862549, -2.49990749359, 171.600006104);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00203129695728, 18.4652061462, -2.0, 171.303192139);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00204883422703, 18.5558929443, -4.97945642471, 177.800003052);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00308686192147, 15.0250864029, -0.294474393129, 168.199996948);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00116629886907, 16.4473133087, -0.777321338654, 169.399993896);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00159040431026, 14.6813001633, 0.247619539499, 167.0);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00174588069785, 14.8472290039, 0.333509385586, 168.199996948);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00197048136033, 14.9785747528, 0.681685388088, 169.399993896);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00225690030493, 15.3383541107, 1.16889095306, 170.600006104);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00355353788473, 16.0062713623, 1.6973361969, 171.800003052);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00251726340503, 17.0923156738, 2.586820364, 173.0);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00111060228664, 17.9681110382, 3.21410918236, 174.0);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.000558144121896, 17.5627937317, 2.84550333023, 174.0);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(1.81993309525e-05, 17.7988529205, 3.21594405174, 173.98916626);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00183286971878, 14.6828136444, 0.239969462156, 167.0);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(1.55933328472e-08, 16.0334968567, -5.14361858368, 183.800003052);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.000613433599938, 16.8454933167, -8.0, 185.847961426);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.000392660062062, 16.8967227936, -8.17721843719, 185.97428894);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.000216763059143, 16.0278873444, -5.17160415649, 183.831787109);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.000510998885147, 17.6196708679, 2.76399850845, 173.999572754);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.000510998885147, 17.6196708679, 2.76399850845, 173.999572754);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00218025548384, 18.5728797913, -5.00091981888, 177.800003052);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00170071551111, 14.1904087067, -0.00075072637992, 162.199996948);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00207023718394, 14.2773313522, 0.203762307763, 163.399993896);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(1.46850497913e-05, 19.9968929291, 6.04518461227, 177.592880249);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.000729255320039, 20.0149917603, -6.0, 178.545806885);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.000519823282957, 20.2634048462, -6.13419532776, 178.439758301);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.000345350534189, 46.0, -14.5436410904, 189.518951416);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00153938634321, 46.111366272, -14.5555858612, 189.498001099);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.000753535656258, 38.1833190918, 6.98196220398, 199.399993896);
		debugHits_150.push_back(debugHit);
		debugHit = new EcalHit(0.00039651003317, 38.0863265991, 7.99897480011, 199.800796509);
		debugHits_150.push_back(debugHit);

		std::vector<EcalHit*> debugHits_500;
		// 3rd cluster  - 500 MeV
		debugHit = new EcalHit(0.0018122785259, -14.295507431, -0.000170822124346, 163.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00177316681948, -14.3883504868, -0.00812062248588, 164.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00153593497816, -14.4872055054, -0.0201545637101, 165.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000132212720928, -14.5002946854, 3.19189112962e-17, 166.703598022);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00157521944493, -14.5084199905, 0.0157580450177, 167.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00206748070195, -14.5455999374, 0.106160879135, 168.199996948);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00186321442015, -14.5986375809, 0.239340811968, 169.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00170108163729, -14.5987539291, 0.364597409964, 170.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00157172372565, -14.5277204514, 0.608340620995, 171.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00189113197848, -14.4984807968, 0.740567564964, 173.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00134592398535, -14.5281848907, 1.00714170933, 174.199996948);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00127887236886, -14.4933767319, 0.641091942787, 172.336303711);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00135394290555, -14.5575656891, 0.135288342834, 168.462600708);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00047312406241, -44.0, 7.68431425095, 201.367172241);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000689223292284, -45.75756073, 7.87763690948, 201.529418945);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00299006723799, -36.5471000671, 6.86453390121, 200.67918396);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000691315974109, -14.6699924469, 0.379025131464, 176.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(2.68075200438e-05, -14.7145032883, 0.275547891855, 177.580764771);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00275968015194, -14.5100727081, 0.75080704689, 173.076217651);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00195704470389, -15.2699632645, 2.0, 173.655349731);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(2.91009996545e-07, -14.6461801529, 0.390940397978, 170.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00147165905219, -14.6511230469, 0.407032370567, 170.728286743);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0023618619889, -14.7448806763, 0.507861196995, 171.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00246815709397, -14.663271904, 1.03005433083, 173.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00217302236706, -18.7090167999, -2.81317543983, 201.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00108964566607, -18.4115028381, -2.0803706646, 203.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00192407227587, -16.8265361786, 0.27970340848, 181.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0017738963943, -16.8676986694, 0.469205111265, 182.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00177623471245, -17.3541584015, 0.0981931760907, 183.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00289030070417, -17.3221321106, -0.094552397728, 185.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00191981694661, -16.0616168976, -0.0836994200945, 186.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(1.55933328472e-08, -14.5995483398, 0.240008845925, 169.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00184675026685, -14.600522995, 0.243390247226, 169.428985596);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00165402237326, -14.5743713379, 0.334469974041, 170.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00111400522292, -14.1874446869, 0.344221651554, 171.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00163652037736, -14.0, 0.377666175365, 172.129058838);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00121124600992, -14.6293764114, 0.318531095982, 170.13494873);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00066203600727, -14.9259614944, 0.907361388206, 177.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000974709982984, -14.9382257462, 0.878376662731, 178.049026489);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00178433232941, -14.6185131073, 0.207768455148, 169.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000212757804547, -14.6202716827, -0.0991808250546, 170.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0033113011159, -14.6413192749, -1.71303945321e-18, 170.664855957);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000346779561369, -14.6803188324, 0.352478653193, 171.063598633);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00216441275552, -15.2553081512, -0.0300423912704, 175.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00224839663133, -15.3665370941, -0.0155015680939, 176.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00184084591456, -15.4576768875, -0.0183917544782, 177.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00161726260558, -15.4737510681, -0.00316139846109, 179.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00171138066798, -15.5157194138, 0.122766934335, 180.199996948);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00162493111566, -15.6311359406, 0.415858536959, 181.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00269026774913, -15.7255620956, 0.589154660702, 182.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0024508042261, -15.8945159912, 0.830347061157, 183.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00166758405976, -15.9678544998, 0.974763572216, 185.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00227989978157, -15.9288778305, 1.04407119751, 186.199996948);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00175506412052, -15.8100538254, 0.863731563091, 187.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00178758508991, -15.756485939, 0.461116224527, 188.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00202540773898, -15.7799863815, -0.307149559259, 189.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00123095675372, -16.01902771, -0.028697706759, 191.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00202262448147, -14.6045188904, 0.18540649116, 169.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00175317341927, -15.4411115646, 0.563824534416, 175.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00243844720535, -18.705696106, -2.88681197166, 201.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00157669663895, -19.0334072113, -3.24210739136, 202.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000100506971648, -14.296043396, -0.000994362402707, 163.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00158216629643, -14.3018341064, -3.10515495582e-17, 163.448486328);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0017131002387, -14.4212360382, 0.0358741432428, 164.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00180938676931, -14.4259061813, 0.122225888073, 165.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00155487377197, -14.4174442291, 0.254697293043, 167.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0019148401916, -14.4611463547, 0.33683988452, 168.199996948);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0018238282064, -14.5049886703, 0.404577672482, 169.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00199475022964, -14.6011610031, 0.407820612192, 170.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00176100514363, -14.8044786453, 0.380831986666, 171.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00217910646461, -14.8195590973, 0.291282236576, 173.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00165408442263, -14.9817533493, 0.49453663826, 174.199996948);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00195689778775, -15.1594457626, 0.807230949402, 175.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00240894872695, -15.5585365295, 0.427731364965, 176.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00214824732393, -15.1687383652, 0.816417276859, 175.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00160046759993, -16.241361618, 0.893570780754, 176.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00206321594305, -15.2218847275, 0.0211937651038, 175.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00213772105053, -15.7718486786, 0.385159105062, 176.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000372798996978, -38.0, 3.83552360535, 201.316299438);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000206327124033, -38.2227554321, 3.87092161179, 201.524368286);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0019148447318, -17.9032669067, 0.644309222698, 182.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000233952945564, -15.8404216766, 0.212902724743, 177.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000300810555927, -16.0689506531, 0.083506077528, 178.686447144);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00121360248886, -16.0886802673, 0.0766642689705, 178.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000297297694487, -15.1443519592, 0.753711521626, 175.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0008784388192, -15.1677455902, 0.791642427444, 175.572402954);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00201482465491, -14.8099594116, 0.375354796648, 171.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000541722693015, -15.0400505066, 0.434747487307, 172.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00321535114199, -15.2201738358, 0.146785959601, 172.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000180183749762, -14.6567277908, 0.737081348896, 173.167510986);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000386823405279, -14.5155191422, 0.363929629326, 169.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00213812431321, -14.5299186707, 0.373373568058, 169.722793579);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000510998885147, -19.0543575287, -3.81961011887, 202.507751465);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000510998885147, -19.0543575287, -3.81961011887, 202.507751465);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000337167701218, -15.3739957809, -3.83645129204, 206.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(5.60498374398e-05, -14.8267946243, -3.98756194115, 207.391693115);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00128682749346, -14.8149309158, -4.0, 207.432113647);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000412973458879, -16.0522823334, -0.104735814035, 191.508026123);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00232209870592, -15.1100311279, -2.5638024807, 198.199996948);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00214486289769, -14.9936208725, -2.72579073906, 199.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00190932524856, -14.8330230713, -2.58658981323, 200.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00301038729958, -13.9839286804, -2.75712394714, 201.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00167046301067, -14.9395675659, -2.6096405983, 199.970062256);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000491953396704, -19.507982254, 4.55059576035, 195.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000533988175448, -19.5288696289, 4.57053947449, 195.850769043);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00194810668472, -24.3791542053, 11.7142715454, 246.899993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00336653809063, -16.2668418884, -0.0367063842714, 236.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00159525673371, -16.3898162842, 0.341603666544, 236.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00178622617386, -15.0889778137, -2.59781384468, 198.199996948);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00194888922852, -14.7000007629, -3.01035642624, 199.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00151036900934, -14.47352314, -3.56731081009, 200.600006104);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00193780998234, -14.810128212, 0.373438864946, 171.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0015940394951, -15.4226074219, -0.00836652517319, 177.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00179370341357, -15.2867746353, -0.00135269458406, 179.0);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00026762651396, -14.977347374, -0.120556607842, 180.199996948);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00127140700351, -15.0694122314, 5.5175047896e-17, 180.267562866);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(1.55933328472e-08, -13.9169101715, -0.92998945713, 183.800003052);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000781193783041, -13.6822462082, -1.10673010349, 184.597854614);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0012842612341, -14.4949483871, -0.334768414497, 181.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00169688044116, -14.0, -0.206062689424, 181.841400146);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.0013393178815, -13.9359149933, -0.1902898103, 181.877960205);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(1.55933328472e-08, -15.0494318008, 0.359954118729, 187.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(1.55933328472e-08, -17.0421276093, -2.03847670555, 198.199996948);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000952094385866, -17.130947113, -2.14507555962, 198.638793945);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000519918045029, -15.0182313919, 0.390556544065, 188.201461792);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00169433932751, -14.9196872711, 0.565600395203, 188.399993896);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000236668784055, -17.3024711609, 13.5609493256, 180.199996948);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00100639881566, -17.3117675781, 13.7104616165, 180.293395996);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000510998885147, -14.3660469055, -3.39837265015, 200.946182251);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.000510998885147, -14.3660469055, -3.39837265015, 200.946182251);
		debugHits_500.push_back(debugHit);
		debugHit = new EcalHit(0.00274928542785, -14.4493122101, -0.341720342636, 181.399993896);
		debugHits_500.push_back(debugHit);


		vector<EcalHit*> cells_50 = EcalHit::AddCells(debugHits_50);
		vector<EcalHit*> cells_150 = EcalHit::AddCells(debugHits_150);
		vector<EcalHit*> cells_500 = EcalHit::AddCells(debugHits_500);

		cout << " 50 MeV: Merged " << cells_50.size() << " hits to " << cells_50.size() << " cells." << endl;
		cout << "150 MeV: Merged " << cells_150.size() << " hits to " << cells_150.size() << " cells." << endl;
		cout << "500 MeV: Merged " << cells_500.size() << " hits to " << cells_500.size() << " cells." << endl;


		for(const auto& hit : cells_50)
			cells.push_back(hit);
		for(const auto& hit : cells_150)
			cells.push_back(hit);
		for(const auto& hit : cells_500)
			cells.push_back(hit);


		vector<EcalCluster*> clusters_50 = EcalCluster::GetClusters(cells_50);
		vector<EcalCluster*> clusters_150 = EcalCluster::GetClusters(cells_150);
		vector<EcalCluster*> clusters_500 = EcalCluster::GetClusters(cells_500);

		cout << " 50 MeV: Merged " << cells_50.size() << " cells to " << clusters_50.size() << " clusters." << endl;
		cout << "150 MeV: Merged " << cells_150.size() << " cells to " << clusters_150.size() << " clusters." << endl;
		cout << "500 MeV: Merged " << cells_500.size() << " cells to " << clusters_500.size() << " clusters." << endl;


		for(const auto& cluster : clusters_50)
			clusters.push_back(cluster);
		for(const auto& cluster : clusters_150)
			clusters.push_back(cluster);
		for(const auto& cluster : clusters_500)
			clusters.push_back(cluster);

	}


	func->MakeThetaPhiMap("Cells_ThetaPhi", cells);


	vector<pair<float,float> > extraPoints;
	for(unsigned int i=0;i<clusters.size();i++){
		pair<float,float> extraPoint(clusters[i]->GetPhi(), clusters[i]->GetTheta());
		extraPoints.push_back(extraPoint);
	}


	func->MakeThetaPhiMap("Clusters_ThetaPhi", clusters, extraPoints);

	func->DrawEventDisplay("EventDisplay", clusters);


	if(infile != 0){
		infile->Close();
		delete infile;
	}

	return 0;

}
