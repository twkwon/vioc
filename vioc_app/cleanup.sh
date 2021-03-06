# select
while true; do
	read -p "Do you want to remove all maintainer files [y/n]? " yn
	case $yn in
		[Yy]* ) break;;
		[Nn]* ) exit;;
		* ) echo "Please answer y or n.";;
	esac
done

# dirs
rm -rf autom4te.cache/
rm -rf m4/
rm -rf src/prog/.deps
rm -rf src/util/.deps
rm -rf src/util/.libs
rm -f src/util/*.lo
rm -f src/util/*.la
rm -f src/util/*.a
rm -rf src/vioc/.deps
rm -rf src/vioc/.libs
rm -f src/vioc/*.lo
rm -f src/vioc/*.la
rm -f src/vioc/*.a

# files
if [ -f doc/Makefile ]; then
	rm doc/Makefile
fi
if [ -f doc/Makefile.in ]; then
	rm doc/Makefile.in
fi
if [ -f man/Makefile ]; then
	rm man/Makefile
fi
if [ -f man/Makefile.in ]; then
	rm man/Makefile.in
fi
if [ -f src/Makefile ]; then
	rm src/Makefile
fi
if [ -f src/Makefile.in ]; then
	rm src/Makefile.in
fi
if [ -f src/prog/Makefile ]; then
	rm src/prog/Makefile
fi
if [ -f src/prog/Makefile.in ]; then
	rm src/prog/Makefile.in
fi
if [ -f src/util/Makefile ]; then
	rm src/util/Makefile
fi
if [ -f src/util/Makefile.in ]; then
	rm src/util/Makefile.in
fi
if [ -f src/vioc/Makefile ]; then
	rm src/vioc/Makefile
fi
if [ -f src/vioc/Makefile.in ]; then
	rm src/vioc/Makefile.in
fi

if [ -f aclocal.m4 ]; then
	rm aclocal.m4
fi
if [ -f config.status ]; then
	rm config.status
fi
if [ -f config.log ]; then
	rm config.log
fi
if [ -f config.h ]; then
	rm config.h
fi
if [ -f config.h.in ]; then
	rm config.h.in
fi
if [ -f config.h.in~ ]; then
	rm config.h.in~
fi
if [ -f configure ]; then
	rm configure
fi
if [ -f configure.scan ]; then
	rm configure.scan
fi
if [ -f depcomp ]; then
	rm depcomp 
fi
if [ -f install-sh ]; then
	rm install-sh 
fi
if [ -f missing ]; then
	rm missing
fi
if [ -f Makefile ]; then
	rm Makefile
fi
if [ -f Makefile.in ]; then
	rm Makefile.in
fi
if [ -f stamp-h1 ]; then
	rm stamp-h1
fi

echo "Cleaned files!"
