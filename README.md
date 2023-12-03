# GridCodeManager Enabler

This is a temporary WiP repository to prepare AC V2G EVSE SUC implementation with Grid support (code)
and Energy services and electrical Protection features

This implementation shall respect maximum software modularity and within Solid design principle.  


# Todo list
- [ ] Add Gabarit gauge utilities codes to generate grid code data stocked locally within csv formatting
- [ ] Add unit test to confirm the accuracy convergence limit
- [x] Add core grid code value / class layout design
- [ ] Add some numerical math algebra support
- [ ] Add first time performance profiling support, thread safe improvement in future work
- [ ] Add observer design to bidirectional notify the other entity caller/callee and forward the required input for next computing step
- [ ] Separate concerns of grid code type, criticality and associated energy services by strategy design pattern 
- [ ] Mermaid UML functional sequence scheme
- [ ] Simplified SUC logic charts
- [ ] Software architecture integration scheme within Charge Point firmware
- [ ] Functional unitary workflow specification list for testability purpose
- [ ] Build system improvement by Cmake
- [ ] CI build and test with Cmake workflow 
- [ ] Package improvement by Cmake tooling
- [ ] Improve the code design or smells by dependencies inversion and SOLID principle 
