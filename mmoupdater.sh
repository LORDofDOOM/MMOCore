#!/bin/bash

#########################################################################################################
#
# Copyright (C) 2012 LORDofDOOM <https://github.com/LORDofDOOM>
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>.
#
#########################################################################################################

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# This shell script is for autoupdate your core and database - USE IT AT YOUR OWN RISK
# DO NOT RUN OR EDIT THIS FROM YOUR SOURCE FOLDER !!! COPY IT ANYWHERE ELSE !!!!
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#########################################################################################################
# Start config part
#########################################################################################################

#-------------------------------------------------------------------------------------------------------#
#				INI Parser																				#
#-------------------------------------------------------------------------------------------------------#

# prefix for all global variables
INI_PREFIX=bash_inifile_

function ini_load {
    # param1 inifile
    local tmpfile=`(mktemp "${TMPDIR-/tmp}/bash_inifileXXXXXXXX") 2>/dev/null || echo ${TMPDIR-/tmp}/bash_inifile$$`
    awk -v INI_PREFIX=${INI_PREFIX} -f - "$1" >$tmpfile <<EOF

# default global section
BEGIN {
  FS="[[:space:]]*=[[:space:]]*"
  section="globals";
}

{
 # kill comments 
 sub(/;.*/, "");
}

/^\[[^\]]+\]$/ {
 section=substr(\$0, 2, length(\$0) -2);
 # map section to valid shell variables
 gsub(/[^[[:alnum:]]/, "_", section)
 printf "%s%s_keys=()\n", INI_PREFIX, section, INI_PREFIX, section
 printf "%s%s_values=()\n", INI_PREFIX, section, INI_PREFIX, section
}

\$1 ~ /^[[:alnum:]\._]+\$/ {
 # remove trail/head single/double quotes
 gsub(/(^[\"\']|[\'\"]\$)/, "", \$2);
 # escape inside single quotes 
 gsub(/\47/, "'\"'\"'", \$2);
 printf "%s%s_keys=(\"\${%s%s_keys[@]}\" '%s')\n", INI_PREFIX, section, INI_PREFIX, section, \$1
 printf "%s%s_values=(\"\${%s%s_values[@]}\" '%s')\n", INI_PREFIX, section, INI_PREFIX, section, \$2
}
EOF

while read line ; do
    eval $line
done  <${tmpfile}

rm ${tmpfile}

}

function ini_get_value {
    # param1 section
    # param2 key
    
    # map section to valid bash variable like in awk parsing
    local section=${1//[![:alnum:]]/_}
    local keyarray=${INI_PREFIX}${section}_keys[@]
    local valuearray=${INI_PREFIX}${section}_values[@]
    local keys=("${!keyarray}")
    local values=("${!valuearray}")
    for (( i=0; i<${#keys[@]}; i++ )); do
	if [[ "${keys[$i]}" = "$2" ]]; then
	    echo "${values[$i]}"
	    return 0
	fi
    done
    return 1
}

function ini_get_sections {
    eval local prefix_arrays=\${!$INI_PREFIX\*}
    for varname in $prefix_arrays; do
	# grep for key arrays
	local arrayname=${varname#${INI_PREFIX}}
	# strip trailing keys suffix
	if [[ ${arrayname%*_keys} != ${arrayname} ]]; then
	    echo ${arrayname%*_keys}
	fi
    done
}

function ini_get_keys {
    #param1 section
    # map section to valid bash variable like in awk parsing
    local section=${1//[![:alnum:]]/_}
    local keyarray=${INI_PREFIX}${section}_keys[@]
    local keys=("${!keyarray}")
    echo ${keys[@]}
}

#-------------------------------------------------------------------------------------------------------#
#				Color Codes 																			#
#-------------------------------------------------------------------------------------------------------#

#white='\E[37;40m'
#red='\E[31;40m\033[1m'
#green='\E[32;40m\033[1m'
#yellow='\E[33;40m\033[1m'

red='\E[0;31m'
RED='\E[1;31m'
blue='\E[0;34m'
BLUE='\E[1;34m'
cyan='\E[0;36m'
CYAN='\E[1;36m'
NC='\033[0m'      # no color
black='\E[0;30m'
BLACK='\E[1;30m'
green='\E[0;32m'
GREEN='\E[1;32m'
yellow='\E[0;33m'
YELLOW='\E[1;33m'
magenta='\E[0;35m'
MAGENTA='\E[1;35m'
white='\E[0;37m'
WHITE='\E[1;37m'

alias Reset="tput sgr0"      	# Reset colors #+ without clear command
cecho ()                     	# Color-echo.
								# Argument $1 = Nachricht
								# Argument $2 = Farbe
{
local default_msg="Text fehlt"
message=${1:-$default_msg}   	# default if no echo text
color=${2:-$white}           	# default color is white
	echo -e "$color$message"
	#Reset                      # Reset to normal color
	tput sgr0					# Reset to normal color
	return
} 

#-------------------------------------------------------------------------------------------------------#
#				Script variables																		#
#-------------------------------------------------------------------------------------------------------#

WORKING_DIR=$PWD

if [ "$1" != "" ]; then
	if [ ! -f $1 ];
	then
		echo "$1 not found - Exit"
		exit
	fi
  ini_load $1
else
	if [ ! -f mmoconfig.ini ];
	then
		echo "mmoconfig.ini not found - Exit"
		exit
	fi
  ini_load mmoconfig.ini
fi

#INI LAUNCHER
LAUNCHER_SCREENNAME=`ini_get_value LAUNCHER screenname`
LAUNCHER_USERNAME=`ini_get_value LAUNCHER username`
LAUNCHER_START_IN_DEBUG_MODE=`ini_get_value LAUNCHER start_in_debug_mode`
LAUNCHER_WORLDSERVER_BIN=`ini_get_value LAUNCHER worldserver_bin`
LAUNCHER_WORLDSERVER_CONF=`ini_get_value LAUNCHER worldserver_conf`
WORLDSERVER_FOLDER=`ini_get_value LAUNCHER WORLDSERVER_FOLDER`

#INI DEBUG
DEBUG_MODE=`ini_get_value DEBUG start_in_debug_mode`
DEBUG_CRASHLOG_FOLDER=`ini_get_value DEBUG crashlog_folder`

#INI MAIN
USERNAME=`ini_get_value MAIN USERNAME`
PASSWORD=`ini_get_value MAIN PASSWORD`
TRINITY_WORLD_REALM=`ini_get_value MAIN TRINITY_WORLD_REALM`
TRINITY_CHARACTERS_REALM=`ini_get_value MAIN TRINITY_CHARACTERS_REALM`
TRINITY_WORLD_TEMP=`ini_get_value MAIN TRINITY_WORLD_TEMP`

COMPILE=`ini_get_value MAIN COMPILE`
KILL_RUNNING_WORLDSERVER=`ini_get_value MAIN PASSWORD`
RESTART_SERVER_AFTER_COMPILE=`ini_get_value MAIN RESTART_SERVER_AFTER_COMPILE`
DATABASE=`ini_get_value MAIN DATABASE`
INCLUDE_TESTSERVER_SQL=`ini_get_value MAIN INCLUDE_TESTSERVER_SQL`

URL_GIT_CORE=`ini_get_value MAIN URL_GIT_CORE`
URL_GIT_DATABASE=`ini_get_value MAIN URL_GIT_DATABASE`

MAIN_PATH=`ini_get_value MAIN MAIN_PATH`
BACKUP_PATH=`ini_get_value MAIN BACKUP_PATH`
DATABASE_FOLDER=`ini_get_value MAIN DATABASE_FOLDER`
CORE_FOLDER=`ini_get_value MAIN CORE_FOLDER`
RECOMPILE_OUTBUT_FOLDER=`ini_get_value MAIN RECOMPILE_OUTBUT_FOLDER`
CORE_BUILD_FOLDER=`ini_get_value MAIN CORE_BUILD_FOLDER`
COPY_WORDSERVER_BIN=`ini_get_value MAIN COPY_WORDSERVER_BIN`

DOWNLOAD_CORE_FROM_GIT=`ini_get_value MAIN DOWNLOAD_CORE_FROM_GIT`
RECOMPILE_CORE=`ini_get_value MAIN RECOMPILE_CORE`
REMOVE_CORE_BEFORE_UPDATE=`ini_get_value MAIN REMOVE_CORE_BEFORE_UPDATE`

DOWNLOAD_DATABASE_FROM_GIT=`ini_get_value MAIN DOWNLOAD_DATABASE_FROM_GIT`
ADDITIONAL_SQLS_INI=`ini_get_value MAIN ADDITIONAL_SQLS`
ADDITIONAL_SQLS=($ADDITIONAL_SQLS_INI);
INSTALL_GTDB=`ini_get_value MAIN INSTALL_GTDB`
REMOVE_DATABASE_BEFORE_UPDATE=`ini_get_value MAIN REMOVE_DATABASE_BEFORE_UPDATE`
BACKUP_CURRENT_WORLD_DATABASE=`ini_get_value MAIN BACKUP_CURRENT_WORLD_DATABASE`
BACKUP_CURRENT_CHAR_DATABASE=`ini_get_value MAIN BACKUP_CURRENT_CHAR_DATABASE`
WORLD_DATABASE_CUSTOMS_TEMP_TABLE=`ini_get_value MAIN WORLD_DATABASE_CUSTOMS_TEMP_TABLE`
DROP_OLD_WORLD_DATABASE_CUSTOMS=`ini_get_value MAIN DROP_OLD_WORLD_DATABASE_CUSTOMS`
DROP_OLD_WORLD_DATABASE_BACKUP=`ini_get_value MAIN DROP_OLD_WORLD_DATABASE_BACKUP`
DROP_OLD_CHAR_DATABASE_BACKUP=`ini_get_value MAIN DROP_OLD_CHAR_DATABASE_BACKUP`
OPTIMIZE_DB=`ini_get_value MAIN OPTIMIZE_DB`

#-------------------------------------------------------------------------------------------------------#
#				Script start																			#
#-------------------------------------------------------------------------------------------------------#

if [ "$KILL_RUNNING_WORLDSERVER" = "1" ]; then
sudo -u $LAUNCHER_USERNAME kill `sudo -u $LAUNCHER_USERNAME screen -ls |grep $LAUNCHER_SCREENNAME |awk -F . '{print $1}'|awk '{print $1}'`
fi

# Start compile part
if [ "$COMPILE" = "1" ]; then
#-------------------------------------------------------------------------------------------------------#
#				Core Part																				#
#-------------------------------------------------------------------------------------------------------#

if [ -d $MAIN_PATH ]; then
	cecho "$MAIN_PATH exist - Starting" $green
else 
	cecho "$MAIN_PATH dont exist - Creating" $red
	mkdir $MAIN_PATH
fi 
if [ "$REMOVE_CORE_BEFORE_UPDATE" = "1" ]; then
	cecho "Remove Core" $green
	rm -rf $MAIN_PATH/$CORE_FOLDER
	cecho "MMOCore Clone" $green
	git clone $URL_GIT_CORE
fi

if [ "$DOWNLOAD_CORE_FROM_GIT" = "1" ]; then
	 if [ -d $MAIN_PATH/$CORE_FOLDER ]; then
		cecho "MMOCore Pull" $green
		cd $MAIN_PATH/$CORE_FOLDER 
		git pull
	else 
		cecho "MMOCore Clone (Pull not possible - MMOCore dont exist)" $yellow
		cd $MAIN_PATH
		git clone $URL_GIT_CORE
	fi 
fi
echo ""

#-------------------------------------------------------------------------------------------------------#
#				Core Compile Part																		#
#-------------------------------------------------------------------------------------------------------#

if [ -d $MAIN_PATH/$RECOMPILE_OUTBUT_FOLDER ]; then
	cd $MAIN_PATH/$RECOMPILE_OUTBUT_FOLDER
else 
	mkdir $MAIN_PATH/$RECOMPILE_OUTBUT_FOLDER
	cd $MAIN_PATH/$RECOMPILE_OUTBUT_FOLDER
fi 

cecho "Compile MMOCore" $green
cd $MAIN_PATH/$RECOMPILE_OUTBUT_FOLDER	

if [ "$RECOMPILE_CORE" = "1" ]; then
	rm -rf $MAIN_PATH/$RECOMPILE_OUTBUT_FOLDER/*
	
	if [ -d $CORE_BUILD_FOLDER ]; then
		cd $MAIN_PATH/$RECOMPILE_OUTBUT_FOLDER
	else 
		mkdir $CORE_BUILD_FOLDER
		cd $MAIN_PATH/$RECOMPILE_OUTBUT_FOLDER
	fi 	
fi 

if [ "$DEBUG_MODE" = "1" ]; then
	cd $MAIN_PATH/$RECOMPILE_OUTBUT_FOLDER
	cmake ../$CORE_FOLDER -DPREFIX=$CORE_BUILD_FOLDER -DWITH_COREDEBUG=1
else 
	cd $MAIN_PATH/$RECOMPILE_OUTBUT_FOLDER
	cmake ../$CORE_FOLDER -DPREFIX=$CORE_BUILD_FOLDER
fi 		

make -j8
make install

echo ""

#-------------------------------------------------------------------------------------------------------#
#				Copy bin files to correct folder														#
#-------------------------------------------------------------------------------------------------------#
if [ "$COPY_WORDSERVER_BIN" = "1" ]; then
cecho "Delete old binaries"	$green
rm -rf $WORLDSERVER_FOLDER/worldserver
rm -rf $WORLDSERVER_FOLDER/authserver

cecho "Copy binaries to target"	$green
cp $CORE_BUILD_FOLDER/bin/worldserver $WORLDSERVER_FOLDER/worldserver
cp $CORE_BUILD_FOLDER/bin/authserver $WORLDSERVER_FOLDER/authserver
fi
echo ""
# End compile part
fi

# Start SQL Part
if [ "$DATABASE" = "1" ]; then
#-------------------------------------------------------------------------------------------------------#
#				SQL Part																				#
#-------------------------------------------------------------------------------------------------------#
if [ -d $MAIN_PATH ]; then
	cd $MAIN_PATH
else 
	cecho "MAIN_PATH dont exist - Creating" $yellow
	mkdir $MAIN_PATH
	cd $MAIN_PATH
fi 

#-------------------------------------------------------------------------------------------------------#
#				MMODatabase Pull/Update																	#
#-------------------------------------------------------------------------------------------------------#
if [ "$REMOVE_DATABASE_BEFORE_UPDATE" = "1" ]; then
	cecho "Remove Database" $green
	rm -rf $MAIN_PATH/$DATABASE_FOLDER
	cecho "MMODatabase Clone"
	git clone $URL_GIT_DATABASE
fi

if [ "$DOWNLOAD_DATABASE_FROM_GIT" = "1" ]; then
	 if [ -d $MAIN_PATH/$DATABASE_FOLDER ]; then
		cecho "MMODatabase Pull" $green
		echo ""
		cd $DATABASE_FOLDER
		git pull
	else 
		cecho "MMODatabase Clone (Pull not possible - MMODatabase dont exist)"  $yellow
		echo ""
		cd $MAIN_PATH
		git clone $URL_GIT_DATABASE
	fi 
fi
echo ""

#-------------------------------------------------------------------------------------------------------#
#				Backup current Database 																#
#-------------------------------------------------------------------------------------------------------#
if [ -d $MAIN_PATH/$BACKUP_PATH ]; then
	cecho "Creating SQL Backup"  $cyan
else 
	cecho "Creating SQL Backup"  $cyan
	mkdir $MAIN_PATH/$BACKUP_PATH
fi 

if [ "$DROP_OLD_WORLD_DATABASE_BACKUP" = "1" ]; then
	cecho "Drop old WorldDB backups" $green
	rm -rf $MAIN_PATH/$BACKUP_PATH/worlddb_*.sql
fi

if [ "$DROP_OLD_CHAR_DATABASE_BACKUP" = "1" ]; then
	cecho "Drop old CharDB backups"	$green
	rm -rf $MAIN_PATH/$BACKUP_PATH/chardb_*.sql
fi

if [ "$DROP_OLD_WORLD_DATABASE_CUSTOMS" = "1" ]; then
	cecho "Drop old GOB and spawned NPC backups" $green	
	rm -rf $MAIN_PATH/$BACKUP_PATH/customsdb_*.sql
fi

echo ""

if [ "$BACKUP_CURRENT_WORLD_DATABASE" = "1" ]; then
	cecho "Backup current WorldDB" $green
	mysqldump -h localhost -u $USERNAME -p$PASSWORD $TRINITY_WORLD_REALM > $MAIN_PATH/$BACKUP_PATH/worlddb_`eval date +%Y%m%d`.sql
	echo "WorldDB backup has been successfully created ($MAIN_PATH/$BACKUP_PATH/worlddb_`eval date +%Y%m%d`.sql)"
fi

if [ "$BACKUP_CURRENT_CHAR_DATABASE" = "1" ]; then
	cecho "Backup current CharDB" $green
	mysqldump -h localhost -u $USERNAME -p$PASSWORD $TRINITY_CHARACTERS_REALM > $MAIN_PATH/$BACKUP_PATH/chardb_`eval date +%Y%m%d`.sql
	echo "CharDB backup has been successfully created ($MAIN_PATH/$BACKUP_PATH/chardb_`eval date +%Y%m%d`.sql)"
fi

#-------------------------------------------------------------------------------------------------------#
#				Backup GOB and spawned NPC 																#
#-------------------------------------------------------------------------------------------------------#
if [ "$WORLD_DATABASE_CUSTOMS_TEMP_TABLE" = "1" ]; then
cecho "Backup GOB and spawned NPC"	$green
	create_temp_mysql_table="
	-- Create Table
	CREATE DATABASE IF NOT EXISTS $TRINITY_WORLD_TEMP;
	-- Drop Table
	DROP TABLE if exists $TRINITY_WORLD_TEMP.creature;
	-- Create Creature
	CREATE TABLE $TRINITY_WORLD_TEMP.creature LIKE $TRINITY_WORLD_REALM.creature;
	-- Structure
	INSERT INTO $TRINITY_WORLD_TEMP.creature
	  SELECT * FROM $TRINITY_WORLD_REALM.creature WHERE guid > 9000000 ORDER BY guid LIMIT 1000000000;
	-- AutoIncremental  
	ALTER TABLE $TRINITY_WORLD_TEMP.creature AUTO_INCREMENT =90000000;
	-- Remove Auto Increment Fromt the ID table
	ALTER TABLE $TRINITY_WORLD_TEMP.creature change guid guid int (10) unsigned NOT NULL COMMENT 'Global Unique Identifier';
	-- Create New ID
	ALTER TABLE $TRINITY_WORLD_TEMP.creature AUTO_INCREMENT =9000000; 
	ALTER TABLE $TRINITY_WORLD_TEMP.creature ADD COLUMN guid_new INT(10) UNSIGNED AUTO_INCREMENT UNIQUE AFTER guid;
	-- Put the new guid in place
	UPDATE $TRINITY_WORLD_TEMP.creature SET guid = guid_new;
	-- Remove the New Colum
	ALTER TABLE $TRINITY_WORLD_TEMP.creature DROP COLUMN guid_new;
	-- Put auto incremnt back
	ALTER TABLE $TRINITY_WORLD_TEMP.creature change guid guid int(10) unsigned NOT NULL auto_increment COMMENT 'Global Unique Identifier';

	-- Drop Table
	DROP TABLE if exists $TRINITY_WORLD_TEMP.gameobject;
	-- Create gameobject
	CREATE TABLE $TRINITY_WORLD_TEMP.gameobject LIKE $TRINITY_WORLD_REALM.gameobject;
	-- Structure
	INSERT INTO $TRINITY_WORLD_TEMP.gameobject
	  SELECT * FROM $TRINITY_WORLD_REALM.gameobject WHERE guid > 9000000 ORDER BY guid LIMIT 1000000000;
	-- AutoIncremental  
	ALTER TABLE $TRINITY_WORLD_TEMP.gameobject AUTO_INCREMENT =90000000; 
	-- Remove Auto Increment Fromt the ID table
	ALTER TABLE $TRINITY_WORLD_TEMP.gameobject change guid guid int (10) unsigned NOT NULL COMMENT 'Global Unique Identifier';
	-- Create New ID
	ALTER TABLE $TRINITY_WORLD_TEMP.gameobject AUTO_INCREMENT =9000000; 
	ALTER TABLE $TRINITY_WORLD_TEMP.gameobject ADD COLUMN guid_new INT(10) UNSIGNED AUTO_INCREMENT UNIQUE AFTER guid;
	-- Put the new guid in place
	UPDATE $TRINITY_WORLD_TEMP.gameobject SET guid = guid_new;
	-- Remove the New Colum
	ALTER TABLE $TRINITY_WORLD_TEMP.gameobject DROP COLUMN guid_new;
	-- Put auto incremnt back
	ALTER TABLE $TRINITY_WORLD_TEMP.gameobject change guid guid int(10) unsigned NOT NULL auto_increment COMMENT 'Global Unique Identifier';"

mysql -u $USERNAME -p$PASSWORD $TRINITY_WORLD_TEMP << eof
$create_temp_mysql_table
eof

mysqldump -h localhost -u $USERNAME -p$PASSWORD $TRINITY_WORLD_TEMP --add-drop-table=0 --no-create-info > $MAIN_PATH/$BACKUP_PATH/customsdb_`eval date +%Y%m%d`.sql
echo "WorldDB customs (gameobjects, creatures) backup has been successfully created ($MAIN_PATH/$BACKUP_PATH/customsdb_`eval date +%Y%m%d`.sql)"
echo ""

fi


#-------------------------------------------------------------------------------------------------------#
#				Install WorldDB			 																#
#-------------------------------------------------------------------------------------------------------#
cecho "Drop current WorldDB" $green
mysql -u $USERNAME -p$PASSWORD $TRINITY_WORLD_REALM -e "show tables" | grep -v Tables_in | grep -v "+" | gawk '{print "drop table " $1 ";"}' | mysql -u $USERNAME -p$PASSWORD $TRINITY_WORLD_REALM

cecho "Install YTDB" $green
for file in $MAIN_PATH/$DATABASE_FOLDER/YTDB/world_full/*.sql ; do
    cecho Import "$file" $green
	mysql -h localhost -u $USERNAME -p$PASSWORD $TRINITY_WORLD_REALM <"$file"
done
for file in $MAIN_PATH/$DATABASE_FOLDER/YTDB/world_updates/*.sql ; do
    cecho Import "$file" $green
	mysql -h localhost -u $USERNAME -p$PASSWORD $TRINITY_WORLD_REALM <"$file"
done

#-------------------------------------------------------------------------------------------------------#
#				Install German GTDB		 																#
#-------------------------------------------------------------------------------------------------------#
if [ "$INSTALL_GTDB" = "1" ]; then
	cecho "Install GTDB" $green
	for file in $MAIN_PATH/$DATABASE_FOLDER/GTDB/*.sql ; do
		cecho Import "$file" $green
		mysql -h localhost -u $USERNAME -p$PASSWORD $TRINITY_WORLD_REALM <"$file"
	done
fi

#-------------------------------------------------------------------------------------------------------#
#				Import GOB and spawned NPC from Backup													#
#-------------------------------------------------------------------------------------------------------#
if [ "$WORLD_DATABASE_CUSTOMS_TEMP_TABLE" = "1" ]; then
mysql -h localhost -u $USERNAME -p$PASSWORD $TRINITY_WORLD_REALM < $MAIN_PATH/$BACKUP_PATH/customsdb_`eval date +%Y%m%d`.sql
fi

#-------------------------------------------------------------------------------------------------------#
#				Import additional SQLs																	#
#-------------------------------------------------------------------------------------------------------#
cecho "Import additional SQLs" $green
for((i=0;i<${#ADDITIONAL_SQLS};i++))
do 
	if [ "${ADDITIONAL_SQLS[$i]}" != "" ]; then
		echo ${ADDITIONAL_SQLS[$i]}
		for file in $MAIN_PATH/$CORE_FOLDER/${ADDITIONAL_SQLS[$i]}/*.sql
		do
			echo Import "$file"
			mysql -h localhost -u $USERNAME -p$PASSWORD $TRINITY_WORLD_REALM <"$file"
		done
	fi
done

#-------------------------------------------------------------------------------------------------------#
#				Import Testserver only SQLs																#
#-------------------------------------------------------------------------------------------------------#
if [ "$INCLUDE_TESTSERVER_SQL" = "1" ]; then
cecho "Import TESTSERVER SQLs" $red
	for file in $MAIN_PATH/$CORE_FOLDER/sql/updates/mmo_updates_world/TESTSERVER_ONLY/*.sql ; do
		cecho Import "$file" $green
		mysql -h localhost -u $USERNAME -p$PASSWORD $TRINITY_WORLD_REALM <"$file"
	done
fi

#-------------------------------------------------------------------------------------------------------#
#				Set AUTO_INCREMENT DB values															#
#-------------------------------------------------------------------------------------------------------#
cecho "Reset AUTO_INCREMENT on WorldDB"	$green
	create_temp_mysql_table="
	ALTER TABLE $TRINITY_WORLD_REALM.creature AUTO_INCREMENT =9900000; 
	ALTER TABLE $TRINITY_WORLD_REALM.gameobject AUTO_INCREMENT =9900000; 
	ALTER TABLE $TRINITY_WORLD_REALM.game_tele AUTO_INCREMENT =9900000;"

mysql -u $USERNAME -p$PASSWORD $TRINITY_WORLD_REALM << eof
$create_temp_mysql_table
eof

#-------------------------------------------------------------------------------------------------------#
#				Repair and optimize DB	 																#
#-------------------------------------------------------------------------------------------------------#

if [ "$OPTIMIZE_DB" = "1" ]; then
mysqlcheck -h localhost -u $USERNAME -p$PASSWORD $TRINITY_WORLD_REALM --check --analyze --auto-repair --compress --optimize --extended
mysqlcheck -h localhost -u $USERNAME -p$PASSWORD $TRINITY_CHARACTERS_REALM --check --analyze --auto-repair --compress --optimize --extended
fi

# End SQL Part
fi

#-------------------------------------------------------------------------------------------------------#
#				RESTART_SERVER_AFTER_COMPILE															#
#-------------------------------------------------------------------------------------------------------#
cecho "Restarting Core"	$green
if [ "$RESTART_SERVER_AFTER_COMPILE" = "1" ]; then
	cd $WORKING_DIR
	exec ./mmorestarter.sh
	exit 0
fi