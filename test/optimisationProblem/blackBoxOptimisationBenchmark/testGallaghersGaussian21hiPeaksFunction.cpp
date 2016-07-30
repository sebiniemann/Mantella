// Catch
#include <catch.hpp>
#include "../../catchHelper.hpp"

class TestGallaghersGaussian21hiPeaksFunction : public mant::bbob::GallaghersGaussian21hiPeaksFunction {
 public:
  using mant::bbob::GallaghersGaussian21hiPeaksFunction::GallaghersGaussian21hiPeaksFunction;

  // Increases the visibility of internal parameters, to make them accessible.
  using mant::bbob::GallaghersGaussian21hiPeaksFunction::localParameterConditionings_;
  using mant::bbob::GallaghersGaussian21hiPeaksFunction::localParameterTranslations_;
  using mant::bbob::GallaghersGaussian21hiPeaksFunction::rotationQ_;
};

SCENARIO("bbob::GallaghersGaussian21hiPeaksFunction.getObjectiveFunctions", "[bbob::GallaghersGaussian21hiPeaksFunction][bbob::GallaghersGaussian21hiPeaksFunction.getObjectiveFunctions]") {
  GIVEN("A parameter") {
    THEN("Return its objective value") {
      TestGallaghersGaussian21hiPeaksFunction optimisationProblem(2);

      optimisationProblem.localParameterConditionings_ = arma::mat::fixed<2, 21>({{0.0316000000000000, 0.0941000000000000, 0.695200000000000, 0.0379000000000000, 0.0785000000000000, 0.0455000000000000, 0.403000000000000, 0.0546000000000000, 0.162400000000000, 0.833800000000000, 0.112900000000000, 0.0316000000000000, 0.483300000000000, 0.135400000000000, 0.233600000000000, 1, 0.280100000000000, 0.579600000000000, 0.0654000000000000, 0.336000000000000, 0.194700000000000}, {31.6228000000000, 10.6247000000000, 1.43840000000000, 26.3665000000000, 12.7427000000000, 21.9839000000000, 2.48160000000000, 18.3298000000000, 6.15850000000000, 1.19940000000000, 8.85870000000000, 31.6228000000000, 2.06910000000000, 7.38620000000000, 4.28130000000000, 1, 3.56970000000000, 1.72520000000000, 15.2831000000000, 2.97640000000000, 5.13480000000000}});
      optimisationProblem.localParameterTranslations_ = arma::mat::fixed<2, 21>({{3.88046988006379, 3.48513756289102, -0.270161730003235, 4.85084323741215, 4.09273708456685, 4.06956555518155, 2.48403968617775, -3.05985646535702, 2.86023390416098, 2.83681737223021, -1.21255261496145, -4.84980295783483, -1.13538197648007, -0.235504630667314, -1.89377783450836, -0.141184365439883, -2.01025714318263, 2.20188508597522, -1.87187182051243, -1.43803316777208, -3.32024769022262}, {4.07731668346777, 4.69057752770314, 4.30003020022534, -3.25938511897999, -1.14407369651869, -4.55925725633675, -3.18463501051016, -3.50734159723927, -3.86946314768007, -2.77750042261816, -3.75887282263036, 4.10027335886572, -2.43159357320876, -1.67695984324463, -3.08040472463908, -0.0235436608203160, 3.03032870063829, -3.22653220612820, 1.84383546604690, -3.56525764436489, 1.97566093599369}});
      optimisationProblem.rotationQ_ = mant::rotationMatrix2d(0.1);

      CHECK(optimisationProblem.getObjectiveFunctions().at(0).first({1.0, -2.0}) == Approx(20.8599297619));
    }
  }

  THEN("Return the objective function name") {
    mant::bbob::GallaghersGaussian21hiPeaksFunction optimisationProblem(2);

    CHECK(optimisationProblem.getObjectiveFunctions().size() == 1);
    CHECK(optimisationProblem.getObjectiveFunctions().at(0).second == "BBOB Gallagher's Gaussian 21-hi Peaks Function (f22)");
  }
}
