for each in `find $1 ! -type d | sort | sed s/\\\\//\\\\\\\\/ | sed s/\\\\//\\\\\\\\/ | sed s/\\\\//\\\\\\\\/ | sed s/\\\\//\\\\\\\\/ | sed s/\\\\//\\\\\\\\/ | sed s/\\\\//\\\\\\\\/ | sed s/\\\\//\\\\\\\\/ | sed s/\\\\//\\\\\\\\/`
	do 
	if [ ! -z `echo $each | grep tcl` ]
	then
		echo -n ';'
	fi

	# No longer necessary
	# http://support.microsoft.com/kb/326922
	# The shared CRT DLL has been distributed by Microsoft in the 
	# past as a shared system component. This may cause problems when 
	# you run applications that are linked to a different version of 
	# the CRT on computers that do not have the correct versions of 
	# the CRT DLL installed. This is commonly referred to as the "DLL 
	# Conflict" problem.
	# 
	# To address this issue, the CRT DLL is no longer considered a	# system file, therefore, distribute the CRT DLL with any
	# application that relies on it. Because it is no longer a system	# component, install it in your applications Program Files
	# directory with other application-specific code. This prevents 
	# your application from using other versions of the CRT library 
	# that may be installed on the system paths.
	#
	# if [ ! -z `echo $each | grep msvcr71.dll` ]
	# then
	# 	echo -n ';'
	# fi
	echo -n ' File '
	echo $each
	done
