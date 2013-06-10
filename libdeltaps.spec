
# date of git snapshot
%define snapshot .20130610git

Summary: 	Library for controlling power supplies from Delta Elektronika
Name:    	libdeltaps
Version: 	1.1.0
# post-release
# see http://fedoraproject.org/wiki/Packaging:NamingGuidelines#Non-Numeric_Version_in_Release
# 
Release:	1%{?snapshot}%{?dist}.maxlab
License: 	GPL
URL:		http://www.maxlab.lu.se
Group:		System Environment/Libraries
Source: 	%{name}-%{version}.tar.gz
Requires:	libyat
BuildRequires: libyat-devel
# for pogo Makefile templates:
BuildRequires: tango-java
Requires(post):	/sbin/ldconfig

%description
Library for controlling power supplies from Delta Elektronika
via ethernet

%package devel
Summary:	Header files and libraries needed for %{name} development
Group:		Development/Libraries
Requires:	%{name} = %{version}-%{release}
Requires: 	libyat-devel
AutoReqProv: no

%description devel
Header files and static libraries for developing programs using %{name}.

%prep
%setup -q

%build
make -C src/ RELEASE_TYPE=OPTIMIZED OUTPUT_TYPE=SHARED_LIB clean all
make -C src/ RELEASE_TYPE=OPTIMIZED OUTPUT_TYPE=STATIC_LIB clean all

%install
[ -z %{buildroot} ] || rm -rf %{buildroot}

## install libs
read MAJOR MINOR REVISION <<< $(echo %{version} | sed 's/\./ /g')
pushd lib > /dev/null
for f in *.so; do
	install -D -m644 $f %{buildroot}%{_libdir}/$f.$MAJOR.$MINOR.$REVISION
	ln -s $f.$MAJOR.$MINOR.$REVISION %{buildroot}%{_libdir}/$f.$MAJOR.$MINOR
	ln -s $f.$MAJOR.$MINOR.$REVISION %{buildroot}%{_libdir}/$f.$MAJOR
	ln -s $f.$MAJOR.$MINOR.$REVISION %{buildroot}%{_libdir}/$f
done
for f in *.a; do
	install -D -m644 $f %{buildroot}%{_libdir}/$f
done
popd > /dev/null

## install headers
pushd src > /dev/null
for f in *.h; do
	install -D -m644 $f %{buildroot}%{_includedir}/$f
done
popd > /dev/null

%clean
[ -z %{buildroot} ] || rm -rf %{buildroot}

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

# main package includes libraries and copyright info
%files
%defattr (-,root,root,755)
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,755)
%{_libdir}/*.a
%{_libdir}/*.so
%{_includedir}/*

%changelog
* Mon Jun 10 2013 Mirjam Lindberg <mirjam.lindberg@maxlab.lu.se> 1.1.0-1.20130610git
- Changed tolerance from hardcoded value to property. 
- Added some exception handling.
* Mon Jun 03 2013 Andreas Persson <andreas_g.persson@maxlab.lu.se> 1.0.0-1.20130603git
- initial package
