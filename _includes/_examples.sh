for filepath in */_examples/*.sh; do
  pushd "$(dirname "./$filepath")"
  "./$(basename "./$filepath")"
  popd
done