#!/bin/bash
	 
	cd doc/
	doxygen doxygen_conf
	sphinx-build . _build/
	
	rm -rf xml
	
	open _build/mantella.html
exit 0	