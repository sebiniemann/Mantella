#include <cstdlib>
#include <iostream>

#include <armadillo>

#include <parallelKinematicMachine_3prrr_intersectionBased.hpp>
#include <parallelKinematicMachine_3prrr_trigonometryBased.hpp>
#include <parallelKinematicMachine_6prus_intersectionBased.hpp>
#include <parallelKinematicMachine_6prus_trigonometryBased.hpp>

int main(int argc, char* argv[]) {
  ::ParallelKinematicMachine_3PRRR_IntersectionBased pkm_3PRRR_intersectionBased;
  std::cout << pkm_3PRRR_intersectionBased.getJacobian(arma::Col<double>::fixed<3>({0.5, 0.5, 0.0}), arma::Col<double>::fixed<3>({0.5, 0.5, 0.5})) << std::endl;

  ::ParallelKinematicMachine_3PRRR_TrigonometryBased pkm_3PRRR_trigonometryBased;
  std::cout << pkm_3PRRR_trigonometryBased.getJacobian(arma::Col<double>::fixed<6>({0.5, 0.5, 0.0}), arma::Col<double>::fixed<3>({0.5, 0.5, 0.5})) << std::endl;

  ::ParallelKinematicMachine_6PRUS_IntersectionBased pkm_6PRUS_intersectionBased;
  std::cout << pkm_6PRUS_intersectionBased.getJacobian(arma::Col<double>::fixed<3>({0.5, 0.5, 0.5, 0.0, 0.0, 0.0}), arma::Col<double>::fixed<3>({0.5, 0.5, 0.5, 0.5, 0.5, 0.5})) << std::endl;

  ::ParallelKinematicMachine_6PRUS_TrigonometryBased pkm_6PRUS_trigonometryBased;
  std::cout << pkm_6PRUS_trigonometryBased.getJacobian(arma::Col<double>::fixed<6>({0.5, 0.5, 0.5, 0.0, 0.0, 0.0}), arma::Col<double>::fixed<3>({0.5, 0.5, 0.5, 0.5, 0.5, 0.5})) << std::endl;

  return EXIT_SUCCESS;
}
