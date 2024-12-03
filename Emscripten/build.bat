copy Template\\manifest.json Build\\manifest.json
copy Template\\sw.js Build\\sw.js
copy Template\\icon.png Build\\icon.png

emcc build.cpp -o Build/index.html -fno-rtti -fno-exceptions -sUSE_WEBGL2 --shell-file="Template/shell.html" -s ALLOW_MEMORY_GROWTH=1 -s INITIAL_MEMORY=134217728 -Wno-nontrivial-memaccess -Wno-format-security -Wno-dynamic-class-memaccess --js-library build.js -sEXPORTED_RUNTIME_METHODS=setValue -s EXPORTED_FUNCTIONS="['_main', '_malloc', '_free']"