# TODO

 - [ ] Tables
	- [x] Manual data entry
	- [x] Fix bug where symbols and units are desynchronized after removing a column
	- [ ] Possibility to add expressions directly in the new project window
	- [x] Right-click context popup to remove or modify a column
	- [ ] Fix a bug where dependant variables don't all update after adding a new row

- [ ] Expressions
	- [ ] Add possibility to retrieve a value from another line (like "x[i-1]")

- [x] Derivatives
	- [x] Add derivatives
	
- [x] Integrals
	- [x] Add integrals

- [ ] Graphs
	- [x] 2D graphs
	- [ ] 3D graphs
	- [x] Multiple graphs management
	- [x] Multiple data plots per graph
	- [x] Customisable lines
	- [ ] Resize graph to fit window's size.
	- [x] Compute statistics (average, uncertainty, error percentage, etc...)

- [ ] Modeling
	- [ ] Customize the modeling plot
	- [x] Modeling for linear and affine functions
	- [ ] Modeling for more complex functions

- [ ] Translation
	- [ ] Add class and methods which can translate a string of a label ID, such as "welcome.datamorph" into a string corresponding to the chosen language. The class can be something like `Translator(Language* language)` which has a method like `translate(std::string id)`
	- [ ] Add methods for loading all localizations from files under "resources/localized/...". Example : "en.loc" or "fr.loc"
	- [ ] English
	- [ ] Français
	- [ ] Deutsch
	- [ ] Español
	- [ ] Русский
	- [ ] українська
	- [ ] 廣州話 (Cantonese)
	- [ ] 简体中文 (Simplified chinese)

- [ ] Resources
	- [	] Embed resources in the executable
