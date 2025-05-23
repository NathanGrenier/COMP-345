---
CPAN:
  build_dir: cpan/build
  cpan_home: cpan
  ftp: ''
  histfile: cpan/histfile
  keep_source_where: cpan/sources
  make: c/bin/gmake.exe
  make_install_make_command: c/bin/gmake.exe
  makepl_arg: ''
  patch: c/bin/patch.exe
  prefs_dir: cpan/prefs
minicpan:
  local: minicpan
  remote: http://cpan.strawberryperl.com/
  force: 1
  skip_perl: 1
  no_conn_cache: 1
HomeDir:
  my_home: data
  my_data: data
  my_documents: data
Config:
  archlib: perl/lib
  archlibexp: perl/lib
  bin: perl/bin
  binexp: perl/bin
  incpath: c/include
  installarchlib: perl/lib
  installbin: perl/bin
  installhtml1dir: ''
  installhtml3dir: ''
  installhtmldir: ''
  installhtmlhelpdir: ''
  installman1dir: ''
  installman3dir: ''
  installprefix: perl
  installprefixexp: perl
  installprivlib: perl/lib
  installscript: perl/bin
  installsitearch: perl/site/lib
  installsitebin: perl/site/bin
  installsitehtml1dir: ''
  installsitehtml3dir: ''
  installsitelib: perl/site/lib
  installsiteman1dir: ''
  installsiteman3dir: ''
  installsitescript: 'perl/site/bin'
  installstyle: perl/lib
  installusrbinperl: ~
  installvendorarch: 'perl/vendor/lib'
  installvendorbin: 'perl/bin'
  installvendorhtml1dir: ''
  installvendorhtml3dir: ''
  installvendorlib: 'perl/vendor/lib'
  installvendorman1dir: ''
  installvendorman3dir: ''
  installvendorscript: 'perl/bin'
  ld: g++.exe
  _libpthfix_part1: c/lib
  _libpthfix_part2: c/x86_64-w64-mingw32/lib
  _libpthfix_part3: c/lib/gcc/x86_64-w64-mingw32/13.1.0
  lddlflags: '-mdll -s -L"$archlib\CORE" -L"$_libpthfix_part1"'
  ldflags: '-s -L"$archlib\CORE" -L"$_libpthfix_part1"'
  ldflags_nolargefiles: '-s -L"$archlib\CORE" -L"$_libpthfix_part1"'
  libpth: $_libpthfix_part1 $_libpthfix_part2 $_libpthfix_part3
  perlpath: perl/bin/perl.exe
  prefix: perl
  prefixexp: perl
  privlib: perl/lib
  privlibexp: perl/lib
  scriptdir: perl/bin
  scriptdirexp: perl/bin
  sitearch: perl/site/lib
  sitearchexp: perl/site/lib
  sitebin: perl/site/bin
  sitebinexp: perl/site/bin
  sitelib: perl/site/lib
  sitelibexp: perl/site/lib
  siteprefix: perl/site
  siteprefixexp: perl/site
  man1dir: ''
  man1direxp: ''
  man3dir: ''
  man3direxp: ''
  vendorarch: perl/vendor/lib
  vendorarchexp: perl/vendor/lib
  vendorbin: perl/bin
  vendorbinexp: perl/bin
  vendorhtml1dir: ''
  vendorhtml3dir: ''
  vendorlib: perl/vendor/lib
  vendorlibexp: perl/vendor/lib
  vendorman1dir: ''
  vendorman3dir: ''
  vendorprefix: perl/vendor
  vendorprefixexp: perl/vendor
  vendorscript: perl/bin
  sitescript: perl/site/bin
  sitescriptexp: perl/site/bin
  vendorscriptexp: perl/bin
  usrinc: c/include
Env:
  PATH:
    - c/bin
    - perl/bin
