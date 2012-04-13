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

#########################################################################################################
# Main Settings
#
# USERNAME: The MySQL User for the Database Import
# PASSWORD: The MySQL Password for the Database Import
# TRINITY_WORLD_REALM: Your Trinity World DB Name
# TRINITY_CHARACTERS_REALM: Your Trinity Char DB Name
# TRINITY_WORLD_TEMP: A Temp Folder Name (needed for backup of GOBs, NPC that are spawned from GMs)

USERNAME=your_mysql_username
PASSWORD=your_mysql_password
TRINITY_WORLD_REALM=your_trinity_world_db
TRINITY_CHARACTERS_REALM=your_trinity_char_db
TRINITY_WORLD_TEMP=your_trinity_temp_db

#########################################################################################################
# Fast Settings
#
# COMPILE: Compile Core
# DATABASE: Import Database
# INCLUDE_TESTSERVER_SQL: Include SQL from TESTSERVER_ONLY folder in MMOCore (for Fun/Testserver)

COMPILE=1
DATABASE=1
INCLUDE_TESTSERVER_SQL=0

#########################################################################################################
# GIT Settings
#
# URL_GIT_CORE: Where you download your TrinityCore
# URL_GIT_DATABASE: Where you download your Database

URL_GIT_CORE=https://github.com/LORDofDOOM/MMOCore.git 
URL_GIT_DATABASE=https://github.com/LORDofDOOM/MMODatabase.git 

#########################################################################################################
# Folder Setting
#
# MAIN_PATH:  The Path where to download GIT Files, store backups, build source... (abolute path)
# BACKUP_PATH:  Folder to store your backups (relative to $MAIN_PATH)
# DATABASE_FOLDER:  Folder where to find the database SQLs (relative to $MAIN_PATH)
# CORE_FOLDER: Folder where to find the core sources (relative to $MAIN_PATH)
# RECOMPILE_OUTBUT_FOLDER: Where should the executable files be after successfully build (relative to $MAIN_PATH)
# CORE_BUILD_FOLDER: Where should the core be build (abolute path)
# COPY_WORDSERVER_BIN: Copy the new worldserver and authserver after compile to target path
# COPY_WORDSERVER_TO_FOLDER: Copy worldserver and authserver to this folder - old feiles wil bee deleted (abolute path)

MAIN_PATH=/home/MMO
BACKUP_PATH=MMOBackup
DATABASE_FOLDER=MMODatabase
CORE_FOLDER=MMOCore
CORE_BUILD_FOLDER=/home/MMO/MMOCoreMake
COPY_WORDSERVER_BIN=1
COPY_WORDSERVER_TO_FOLDER=/home/trinity
RECOMPILE_OUTBUT_FOLDER=MMOCoreBuild

#########################################################################################################
# Compile Settings
#
# DOWNLOAD_CORE_FROM_GIT: Download core files from GIT (Pull updates or clone if REMOVE_CORE_BEFORE_UPDATE=1)
# COMPILE_CORE: Should the core be compiled ?
# COMPILE_IN_DEBUG_MODE: Use debug mode in compile
# RECOMPILE_CORE: Completly recompile the core (overrite any of your changes)
# REMOVE_CORE_BEFORE_UPDATE: Remove old core files from GIT (Drop folder $MAIN_PATH/MMOCore)

DOWNLOAD_CORE_FROM_GIT=1
COMPILE_CORE=1
COMPILE_IN_DEBUG_MODE=0
RECOMPILE_CORE=0
REMOVE_CORE_BEFORE_UPDATE=0

#########################################################################################################
# Database Settings
#
# DOWNLOAD_DATABASE_FROM_GIT: Download Database files from GIT (Pull updates or clone if REMOVE_DATABASE_BEFORE_UPDATE=1)
# ADDITIONAL_SQLS: Import Additional SQLs (relative to $CORE_FOLDER) - You can add foldersif needed
# INSTALL_GTDB: Install GTDB (German Database)
# REMOVE_DATABASE_BEFORE_UPDATE: Remove old database files from GIT (Drop folder $MAIN_PATH/MMODatabase -> Force redownloading)
# BACKUP_CURRENT_WORLD_DATABASE: Backuop your current WorldDB
# BACKUP_CURRENT_CHAR_DATABASE: Backuop your current CharDB
# WORLD_DATABASE_CUSTOMS_TEMP_TABLE: Backup and restore customs (gameobjects, creatures -> This will only work after the SECOND time you use this script!)
# DROP_OLD_WORLD_DATABASE_CUSTOMS: Remove old backups of WORLD_DATABASE_CUSTOMS_TEMP_TABLE
# DROP_OLD_WORLD_DATABASE_BACKUP: Remove old backups of WorldDB
# DROP_OLD_CHAR_DATABASE_BACKUP: Remove old backups of CharDB
# OPTIMIZE_DB: Repair and optimize Database (makes Trinity faster)

DOWNLOAD_DATABASE_FROM_GIT=1
ADDITIONAL_SQLS=(
"sql/updates/world_ytdb" 
"sql/updates/mmo_updates_world" 
"sql/updates/mmo_updates_world/optional" 
"sql/updates/fc_updates_world"
);

INSTALL_GTDB=1
REMOVE_DATABASE_BEFORE_UPDATE=0
BACKUP_CURRENT_WORLD_DATABASE=1
BACKUP_CURRENT_CHAR_DATABASE=1
WORLD_DATABASE_CUSTOMS_TEMP_TABLE=1
DROP_OLD_WORLD_DATABASE_CUSTOMS=1
DROP_OLD_WORLD_DATABASE_BACKUP=1
DROP_OLD_CHAR_DATABASE_BACKUP=1
OPTIMIZE_DB=1

#########################################################################################################
# End config part - No need for edit below this point
#########################################################################################################

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

if [ "$COMPILE_CORE" = "1" ]; then
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
		
		if [ "$COMPILE_IN_DEBUG_MODE" = "1" ]; then
			cmake ../$CORE_FOLDER -DPREFIX=$CORE_BUILD_FOLDER -DWITH_COREDEBUG=1
		else 
			cmake ../$CORE_FOLDER -DPREFIX=$CORE_BUILD_FOLDER
		fi 	
	fi 

	make -j8
	make install

fi
echo ""

#-------------------------------------------------------------------------------------------------------#
#				Copy bin files to correct folder														#
#-------------------------------------------------------------------------------------------------------#
if [ "$COPY_WORDSERVER_BIN" = "1" ]; then
cecho "Delete old binaries"	$green
rm -rf $COPY_WORDSERVER_TO_FOLDER/worldserver
rm -rf $COPY_WORDSERVER_TO_FOLDER/authserver

cecho "Copy binaries to target"	$green
cp $CORE_BUILD_FOLDER/bin/worldserver $COPY_WORDSERVER_TO_FOLDER/worldserver
cp $CORE_BUILD_FOLDER/bin/authserver $COPY_WORDSERVER_TO_FOLDER/authserver
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
cecho "Import TESTSERVER SQLs" $red
if [ "$INCLUDE_TESTSERVER_SQL" = "1" ]; then
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