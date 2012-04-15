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
LAUNCHER_SCREENNAME=`ini_get_value LAUNCHER SCREENNAME`
LAUNCHER_USERNAME=`ini_get_value LAUNCHER USERNAME`
LAUNCHER_USE_SCREEN=`ini_get_value LAUNCHER USE_SCREEN`

LAUNCHER_START_IN_DEBUG_MODE=`ini_get_value LAUNCHER START_IN_DEBUG_MODE`
LAUNCHER_WORLDSERVER_BIN=`ini_get_value LAUNCHER WORLDSERVER_BIN`
LAUNCHER_WORLDSERVER_CONF=`ini_get_value LAUNCHER WORLDSERVER_CONF`
WORLDSERVER_FOLDER=`ini_get_value LAUNCHER WORLDSERVER_FOLDER`

#INI DEBUG
DEBUG_MODE=`ini_get_value DEBUG DEBUG_MODE`
DEBUG_CRASHLOG_FOLDER=`ini_get_value DEBUG CRASHLOG_FOLDER`


#-------------------------------------------------------------------------------------------------------#
#				Script start																			#
#-------------------------------------------------------------------------------------------------------#

sudo -u $LAUNCHER_USERNAME kill `sudo -u $LAUNCHER_USERNAME screen -ls |grep $LAUNCHER_SCREENNAME |awk -F . '{print $1}'|awk '{print $1}'`

sleep 5

if [ "$LAUNCHER_USE_SCREEN" = "1" ]; then
sudo -u $LAUNCHER_USERNAME screen -d -m -S $LAUNCHER_SCREENNAME ./mmowatcher.sh
else
cd $WORLDSERVER_FOLDER
./$LAUNCHER_WORLDSERVER_BIN -c $WORLDSERVER_FOLDER/$LAUNCHER_WORLDSERVER_CONF
fi