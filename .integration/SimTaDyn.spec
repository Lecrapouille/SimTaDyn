Name: SimTaDyn
Summary: A GIS in a spreadsheet.
Version: 0.2
Release: 1%{?dist}
Source0: %{name}-%{version}.tar.gz
BuildRoot: %{builddir}/%{name}-root
License: GPLv3+
Group: Development/Tools
BuildRequires: gcc-c++ gtksourceviewmm-devel freeglut-devel glew-devel libdw-devel Mesa-devel Mesa-libGLU-devel libglfw-devel

%description
Geographic information system. Draw and manipulate maps like a spreadsheet.

%prep
%setup -q

%build
make compile-external-libs
make -j4

%install
make install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -fr $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}-%{version}
%dir %{_datadir}/%{name}
%dir %{_datadir}/%{name}/%{version}
%dir %{_datadir}/%{name}/%{version}/data
%dir %{_datadir}/%{name}/%{version}/doc
%{_datadir}/%{name}/%{version}/data/*
%{_datadir}/%{name}/%{version}/doc/*
%{_datadir}/%{name}/%{version}/AUTHORS
%{_datadir}/%{name}/%{version}/ChangeLog
%{_datadir}/%{name}/%{version}/LICENSE
%{_datadir}/%{name}/%{version}/README.md
