#include "model_instantiator.h"
#include "storm/models/sparse/Model.h"
#include "storm-pars/modelchecker/instantiation/SparseDtmcInstantiationModelChecker.h"
#include "storm/models/sparse/StandardRewardModel.h"


#include "storm-pars/transformer/SparseParametricDtmcSimplifier.h"

#include "storm/adapters/RationalFunctionAdapter.h"
#include "storm/modelchecker/propositional/SparsePropositionalModelChecker.h"
#include "storm/modelchecker/results/ExplicitQualitativeCheckResult.h"
#include "storm/modelchecker/results/ExplicitQuantitativeCheckResult.h"
#include "storm/modelchecker/prctl/helper/DsMpiUpperRewardBoundsComputer.h"
#include "storm/modelchecker/prctl/helper/BaierUpperRewardBoundsComputer.h"
#include "storm/models/sparse/Dtmc.h"
#include "storm/models/sparse/StandardRewardModel.h"
#include "storm/solver/MinMaxLinearEquationSolver.h"
#include "storm/solver/Multiplier.h"
#include "storm/utility/vector.h"
#include "storm/utility/graph.h"
#include "storm/utility/NumberTraits.h"

template<typename ValueType> using Model = storm::models::sparse::Model<ValueType>;
template<typename ValueType> using Dtmc = storm::models::sparse::Dtmc<ValueType>;
template<typename ValueType> using Mdp = storm::models::sparse::Mdp<ValueType>;
template<typename ValueType> using Ctmc = storm::models::sparse::Ctmc<ValueType>;
template<typename ValueType> using MarkovAutomaton = storm::models::sparse::MarkovAutomaton<ValueType>;

using namespace storm::modelchecker;

// Model instantiator
void define_model_instantiator(py::module& m) {
    py::class_<storm::utility::ModelInstantiator<Dtmc<storm::RationalFunction>, Dtmc<double>>>(m, "PDtmcInstantiator", "Instantiate PDTMCs to DTMCs")
        .def(py::init<Dtmc<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<Dtmc<storm::RationalFunction>, Dtmc<double>>::instantiate, "Instantiate model with given parameter values")
    ;

    py::class_<storm::utility::ModelInstantiator<Mdp<storm::RationalFunction>,Mdp<double>>>(m, "PMdpInstantiator", "Instantiate PMDPs to MDPs")
        .def(py::init<Mdp<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<Mdp<storm::RationalFunction>, Mdp<double>>::instantiate, "Instantiate model with given parameter values")
    ;

    py::class_<storm::utility::ModelInstantiator<Ctmc<storm::RationalFunction>,Ctmc<double>>>(m, "PCtmcInstantiator", "Instantiate PCTMCs to CTMCs")
        .def(py::init<Ctmc<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<Ctmc<storm::RationalFunction>, Ctmc<double>>::instantiate, "Instantiate model with given parameter values");

    py::class_<storm::utility::ModelInstantiator<MarkovAutomaton<storm::RationalFunction>,MarkovAutomaton<double>>>(m, "PMaInstantiator", "Instantiate PMAs to MAs")
        .def(py::init<MarkovAutomaton<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<MarkovAutomaton<storm::RationalFunction>, MarkovAutomaton<double>>::instantiate, "Instantiate model with given parameter values");
}

void define_model_instantiation_checker(py::module& m) {

    py::class_<SparseInstantiationModelChecker<Dtmc<storm::RationalFunction>, double>, std::shared_ptr<SparseInstantiationModelChecker<Dtmc<storm::RationalFunction>, double>>> bpdtmcinstchecker(m, "_PDtmcInstantiationCheckerBase", "Instantiate pDTMCs to DTMCs and immediately check (base)");
    bpdtmcinstchecker.def("specify_formula", &SparseInstantiationModelChecker<Dtmc<storm::RationalFunction>, double>::specifyFormula, "check_task"_a)
            ;
    py::class_<SparseDtmcInstantiationModelChecker<Dtmc<storm::RationalFunction>, double>, std::shared_ptr<SparseDtmcInstantiationModelChecker<Dtmc<storm::RationalFunction>, double>>> (m, "PDtmcInstantiationChecker", "Instantiate pDTMCs to DTMCs and immediately check", bpdtmcinstchecker)

        .def(py::init<Dtmc<storm::RationalFunction>>(), "parametric model"_a)
        .def("check", [](SparseDtmcInstantiationModelChecker<Dtmc<storm::RationalFunction>, double> &sdimc, storm::Environment const& env, storm::utility::parametric::Valuation<storm::RationalFunction> const& val) -> std::shared_ptr<CheckResult> {return sdimc.check(env,val);}, "env"_a, "instantiation"_a)
        .def("set_graph_preserving", &SparseDtmcInstantiationModelChecker<Dtmc<storm::RationalFunction>, double>::setInstantiationsAreGraphPreserving, "value"_a);


    py::class_<SparseInstantiationModelChecker<Mdp<storm::RationalFunction>, double>, std::shared_ptr<SparseInstantiationModelChecker<Mdp<storm::RationalFunction>, double>>> bpmdpinstchecker(m, "_PMdpInstantiationCheckerBase", "Instantiate pMDPs to MDPs and immediately check (base)");
    bpmdpinstchecker.def("specify_formula", &SparseInstantiationModelChecker<Mdp<storm::RationalFunction>, double>::specifyFormula, "check_task"_a)
            ;
    py::class_<SparseMdpInstantiationModelChecker<Mdp<storm::RationalFunction>, double>, std::shared_ptr<SparseMdpInstantiationModelChecker<Mdp<storm::RationalFunction>, double>>> (m, "PMdpInstantiationChecker", "Instantiate PMDP to MDPs and immediately check", bpmdpinstchecker)

            .def(py::init<Mdp<storm::RationalFunction>>(), "parametric model"_a)
            .def("check", [](SparseMdpInstantiationModelChecker<Mdp<storm::RationalFunction>, double> &sdimc, storm::Environment const& env, storm::utility::parametric::Valuation<storm::RationalFunction> const& val) -> std::shared_ptr<CheckResult> {return sdimc.check(env,val);}, "env"_a, "instantiation"_a)
            .def("set_graph_preserving", &SparseMdpInstantiationModelChecker<Mdp<storm::RationalFunction>, double>::setInstantiationsAreGraphPreserving, "value"_a);
;

}