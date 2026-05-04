Name: ccowsay
Version: 0.1.0
Release: alt1
Summary: Minimal C implementation of cowsay
License: GPL-3.0-or-later
Group: Applications/Text
Url: https://github.com/UsamG1t/ccowsay
Source0: %{name}-%{version}.tar

BuildRequires: gcc

%description
ccowsay prints a user-provided message in a speech (or thought)
bubble and renders an ASCII cow below it.

%prep
%setup

%build
%{__cc} %{optflags} -DDEFAULT_COWDIR='"%_datadir/%name/cows"' \
    -o ccowsay src/*.c

%install
install -Dpm755 ccowsay %buildroot%_bindir/ccowsay
install -d %buildroot%_datadir/%name/cows
install -pm644 cows/*.cow %buildroot%_datadir/%name/cows/
install -Dpm644 README %buildroot%_docdir/%name/README

%files
%_bindir/ccowsay
%dir %_datadir/%name
%dir %_datadir/%name/cows
%_datadir/%name/cows/*.cow
%doc %_docdir/%name/README

%changelog
* Mon May 04 2026 usam <malchevskijsa@basealt.ru> 0.1.0-alt1
- Initial C cowsay AltLinux build.
