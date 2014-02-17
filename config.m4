PHP_ARG_WITH(staticvardumper, for staticvardumper support,
[  --with-staticvardumper             Include staticvardumper support])

if test "$PHP_STATICVARDUMPER" != "no"; then
  PHP_NEW_EXTENSION(staticvardumper, staticvardumper.c, $ext_shared)
fi
