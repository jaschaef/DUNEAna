DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $DIR

#export LC_ALL="de_DE.UTF-8"
export LC_ALL="en_US.UTF-8"
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
export MRB_PROJECT=garsoft
source ${DIR}/ART/devel/localProducts_garsoft_develop_e15_debug/setup
cd ${MRB_BUILDDIR}
mrbsetenv
mrbslp
mrbsetenv
cd ${MRB_TOP}

export fclFolder=${DIR}/ART/devel/localProducts_garsoft_develop_e15_debug/garsoft/v01_01_01/fcl/

export PATH=~/.local/bin:${PATH}

cd $DIR


function run_dune()
{
    if [[ $@ == *"-h"* ]] || [[ "$@" == "" ]]
    then
		 		echo "Usage: 'run_dune <fclFile> -n <number of events>'"
 				# echo "   or: 'run_dune -H ' to show a more detailed HELP text"
    # elif [[ $@ == *"-H"* ]]
		# then
		# 		art -c ${fclFolder}prodgenie.fcl -h
		else
				art -c $@
    fi
}


function read_geometry()
{
   if [[ $@ == *"-h"* ]] || [[ "$@" == "" ]]
    then
				echo "Usage: 'read_geometry <geometry root file>'"
 				echo "   or: 'read_geometry -H ' to show a more detailed HELP text"
	 elif [[ $@ == *"-H"* ]]
	 then
			 art -c ${fclFolder}readoutsimjob.fcl -h
   else
			 art -c ${fclFolder}readoutsimjob.fcl $@
   fi
}


function run_reco()
{
		if [[ $@ == *"-h"* ]] || [[ "$@" == "" ]]
    then
				echo "Usage: 'run_reco <readoutsim root file>'"
 				echo "   or: 'run_reco -H ' to show a more detailed HELP text"
		elif [[ $@ == *"-H"* ]]
		then
				art -c ${fclFolder}recojob.fcl -h
		else
				art -c ${fclFolder}recojob.fcl $@
		fi
}


function show_display()
{
		if [[ $@ == *"-h"* ]] || [[ "$@" == "" ]]
    then
				echo "Usage: 'show_display <reco root file>'"
 				echo "   or: 'show_display -H ' to show a more detailed HELP text"
		elif [[ $@ == *"-H"* ]]
		then
				art -c ${fclFolder}evd3D_Jan.fcl -h
		else
				art -c ${fclFolder}evd3D_Jan.fcl $@
		fi
}

cd $DIR
