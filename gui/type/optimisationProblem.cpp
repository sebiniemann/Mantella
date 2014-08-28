#include <optimisationProblem.hpp>

decltype(OptimisationProblem::_names) OptimisationProblem::_names = {
  {OptimisationProblem::Type::AttractiveSectorFunction, "Attractive sector function"},
  {OptimisationProblem::Type::BentCigarFunction, "Bent Cigar function"},
  {OptimisationProblem::Type::BuecheRastriginFunction, "Büche-Rastrigin function"},
  {OptimisationProblem::Type::CompositeGriewankRosenbrockFunctionF8F2, "Composite Griewank Rosenbrock function F8F2"},
  {OptimisationProblem::Type::DifferentPowersFunction, "Different powers function"},
  {OptimisationProblem::Type::DiscusFunction, "Discus function"},
  {OptimisationProblem::Type::EllipsoidalFunction, "Ellipsoida function"},
  {OptimisationProblem::Type::EllipsoidalFunctionRotated, "Ellipsoidal function, rotated"},
  {OptimisationProblem::Type::GallaghersGaussian101mePeaksFunction, "Gallaghers Gaussian 101-me peaks function"},
  {OptimisationProblem::Type::GallaghersGaussian21hiPeaksFunction, "Gallaghers gaussian 21-hi peaks function"},
  {OptimisationProblem::Type::KatsuuraFunction, "Katsuura function"},
  {OptimisationProblem::Type::LinearSlope, "Linear slope"},
  {OptimisationProblem::Type::LunacekBiRastriginFunction, "Lunacek bi-Rastrigin function"},
  {OptimisationProblem::Type::RastriginFunction, "Rastrigin function"},
  {OptimisationProblem::Type::RastriginFunctionRotated, "Rastrigin function, rotated"},
  {OptimisationProblem::Type::RosenbrockFunction, "Rosenbrock function"},
  {OptimisationProblem::Type::RosenbrockFunctionRotated, "Rosenbrock function, rotated"},
  {OptimisationProblem::Type::SchaffersF7Function, "Schaffers F7 function"},
  {OptimisationProblem::Type::SchaffersF7FunctionIllConditioned, "Schaffers F7 function, ill-conditioned"},
  {OptimisationProblem::Type::SchwefelFunction, "Schwefel function"},
  {OptimisationProblem::Type::SharpRidgeFunction, "Sharp ridge function"},
  {OptimisationProblem::Type::SphereFunction, "Sphere function"},
  {OptimisationProblem::Type::StepEllipsoidalFunction, "Step ellipsoidal function"},
  {OptimisationProblem::Type::WeierstrassFunction, "Weierstrass function"}
};

std::string OptimisationProblem::getName(const Type& type) const {
  return _names.at(type);
}
