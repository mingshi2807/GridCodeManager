# GridCodeManager Enabler

This is a temporary WiP repository to prepare AC V2G EVSE SUC implementation with Grid support (code)
and Energy services and electrical Protection features

This implementation shall respect maximum software modularity and within Solid design principle.  


# Todo list
- [x] Add `Gabarit` gauge utilities codes to generate grid code data stocked locally within csv formatting
- [x] Add unit test to confirm the accuracy convergence limit
- [x] Add core grid code value / class layout design
- [x] Add some numerical math algebra support
- [ ] Add first time performance profiling support, thread safe improvement in future work
- [x] Add event driven (observer) design to notify grid code related event and required service toward to setpoint computing entity 
- [ ] Add factory pattern to create abstracted grid service manager faced to up or downstream components (ex. metering, iEMS) 
- [ ] Separate concerns of grid code type, criticality and associated energy services by strategy design pattern 
- [ ] Mermaid UML functional sequence scheme
- [ ] Simplified SUC logic charts
- [ ] Software architecture integration scheme within Charge Point firmware
- [ ] Functional unitary workflow specification list for testability purpose
- [x] Build system improvement by Cmake
- [x] Add conan packages dependencies manager 
- [ ] CI build and test with Cmake workflow 
- [ ] Software packaging improvement by Cmake tooling Cpack
- [ ] Improve the code design or smells by dependencies inversion and SOLID principle 
