#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"



# link files in LinkMe to corresponding positions
function ReplaceWithLinks(){
		local sourcefolder=$1
		local destfolder=$2

		set --
		# echo "ReplaceWithLinks: $sourcefolder"
		
		if [ ! -e $sourcefolder ]
		then
				# echo "Creating empty Link folder '$sourcefolder'"
				mkdir $sourcefolder
		fi
		
		for f in `\ls $sourcefolder`
		do
				link=${sourcefolder}/${f}
				target=${destfolder}/${f}

				if [[ $link == *~ ]]
				then
						continue
				fi

				#echo " Found:  $f"
				# echo "  => '$link' / '$target'"

				if [ ! -e $target ]
				then
						#echo "   => Create link"
						ln -s $link $target
				elif [ -L $target ]
				then
						rmcmd="rm $target"
						echo "$rmcmd (this was a link)"
						$rmcmd
						
						lncmd="ln -s $link $target"
						echo $lncmd
						$lncmd
						
						lncmd="ln -s $link $target"
						#echo $lncmd
						$lncmd
						
						
				elif [ -f $target ]
				then
						#echo "   => File -> create backup"
						mvcmd="mv $target ${target}_bak"
						echo $mvcmd
						$mvcmd
				else
						#echo "   => Folder"
						ReplaceWithLinks $link $target
				fi
		done
} 


 
#install DUNE framework
cd $DIR
 
echo "Will now install the glorious DUNEAna package"
echo
echo "Starting with ART package..."

if [ ! -d ART ]
then
		echo " Creating ART folder"
    mkdir ART
else
		echo " Removing old installation in ART folder"
    rm -rf ART/*
fi 

cd ART

#export LC_ALL="de_DE.UTF-8"
export LC_ALL="en_US.UTF-8"
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
export MRB_PROJECT=garsoft
mkdir devel
cd devel
mrb newDev -v develop -q e15:debug
source localProducts*/setup
cd srcs
mrb g -d garsoft garsoft-garsoft
cd garsoft
git remote set-url origin ssh://p-garsoft@cdcvs.fnal.gov/cvs/projects/garsoft-garsoft
cd $MRB_BUILDDIR
mrbsetenv
#mrbsetenv
mrb i -j4

# link your specific fcl files (not sure if required to be done here)
cd $DIR
ReplaceWithLinks ${DIR}/LinkMe/ART ${DIR}/ART
cd $MRB_BUILDDIR
mrbslp

echo


echo "Now the Geometry packages..."

# install geometry stuff
cd $DIR

if [ ! -d Geometry ]
then
	echo " Creating Geometry folder"
	mkdir Geometry
else
	echo " Removing old installation in Geometry folder"
	rm -rf Geometry/*
fi

cd Geometry
git clone https://github.com/brettviren/gegede.git
git clone https://github.com/gyang9/dunendggd.git

# link your specific py and cfg files (not sure if required to be done here)
cd $DIR
ReplaceWithLinks ${DIR}/LinkMe/Geometry ${DIR}/Geometry

cd $DIR/Geometry/dunendggd
python2.7 setup.py develop --user

export PATH=~/.local/bin:${PATH}

cd $DIR

echo

# echo "Now building the DUNEAna package itself"
# if [ ! -d build ]
# then
# 	echo " Creating build folder"
# 	mkdir build
# else
# 	echo " Removing old installation in build folder"
# 	rm -rf build/*
# fi

# cd build

# echo " Running cmake"
# cmake ../
# isOkay=$?

# if [ $isOkay -ne 0 ]
# then
# 	echo "Problem running cmake!"
# 	return 1
# fi

# echo
# echo " Running make"
# make
# isOkay=$?

# if [ $isOkay -ne 0 ]
# then
# 	echo "Problem running make!"
# 	return 1
# fi

 

#echo
#echo "Finally linking all files in LinkMe to corresponing locations"



# cd $DIR
# ReplaceWithLinks ${DIR}/LinkMe/ART ${DIR}/ART
# ReplaceWithLinks ${DIR}/LinkMe/Geometry ${DIR}/Geometry


echo
echo "Installation done. You will need to relog and 'source setup.sh' for everything to work properly"
echo "(This is probably something that could be done better)"
