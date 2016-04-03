#include <mantella>
#include <iostream>
#include <keplerian_toolbox/keplerian_toolbox.h>

int main(){
  
  
  double param = 1e5;
  double numberOfRevolutions = 1.0;
  double transferTime = 0.0;
  

  arma::Col<double>::fixed<3> departurePosition = {1000.0, 1500.0, 2500.0};
  arma::Col<double>::fixed<3> arrivalPosition = {1500.0, 800.0, 4000.0};

  double flightDistance = arma::norm(departurePosition - arrivalPosition);

  double departureDistanceFromSun = arma::norm(departurePosition);
  double arrivalDistanceFromSun = arma::norm(arrivalPosition);

  double semiPerimeter = (flightDistance + departureDistanceFromSun + arrivalDistanceFromSun) / 2.0;

  double lambda = std::sqrt(1.0 - flightDistance / semiPerimeter);


  auto timeOfFlightFunction = [&lambda, &numberOfRevolutions, &transferTime](
      const double parameter) {
    double timeOfFlight;    
    if (std::abs(1.0 - parameter) < 1e-12) {
      return arma::datum::nan;
    } else {
      double a = 1.0 / (1.0 - std::pow(parameter, 2.0));
      timeOfFlight = std::abs(a) * std::sqrt(std::abs(a));
      
      if (parameter < 1.0) {
        double alpha = 2.0 * std::acos(parameter);
        double beta = std::copysign(2 * std::asin( std::sqrt( std::pow(lambda, 2.0) / a)), lambda);

        timeOfFlight *= (alpha - std::sin(alpha)) - (beta - std::sin(beta)) + 2.0 * arma::datum::pi * numberOfRevolutions;
      } else {
        double alpha = 2.0 * std::acosh(parameter);
        double beta = std::copysign(2.0 * std::asinh( std::sqrt( std::pow(lambda, 2.0) / -a)), lambda);
        
        timeOfFlight *= (beta - std::sinh(beta)) - (alpha - std::sinh(alpha));
      }
    }

    return (timeOfFlight / 2.0) - transferTime;
  };
  
  std::cout << "mantella tof function result = " << timeOfFlightFunction(param) << std::endl;
  
      
      
      //---------------------------------------------------------------
      
  double tof;
      
  double battin = 0.01;
  double lagrange = 0.2;
  double dist = fabs(param-1);
  if (dist < lagrange && dist > battin) { // We use Lagrange tof expression
    double a = 1.0 / (1.0-param*param);
    if (a>0)	//ellipse
    {
      double alfa = 2.0*acos(param);
      double beta = 2.0 * asin (sqrt(lambda*lambda/a));
      if (lambda<0.0) beta = -beta;
      tof =  ((a * sqrt (a)* ( (alfa - sin(alfa)) - (beta - sin(beta)) + 2.0*arma::datum::pi*numberOfRevolutions)) / 2.0);
    }
    else
    {
      double alfa = 2.0*std::acosh(param);
      double beta = 2.0 * std::asinh(sqrt(-lambda*lambda/a));
      if (lambda<0.0) beta = -beta;
      tof =  ( -a * sqrt (-a)* ( (beta - sinh(beta)) - (alfa - sinh(alfa)) ) / 2.0);
    }       
  }else{
    
    double K = lambda*lambda;
    double E = param*param-1.0;
    double rho = fabs(E);
    double z = sqrt(1+K*E);
    if (dist < battin) { // We use Battin series tof expression
      double eta = z-lambda*param;
      double S1 = 0.5*(1.0-lambda-param*eta);
      double Q;
      
      
      double Sj=1.0;
      double Cj=1.0;
      double err=1.0;
      double Cj1=0.0;
      double Sj1=0.0;
      int j=0;
      while (err > 1e-11){
        Cj1 = Cj*(3.0+j)*(1.0+j)/(2.5+j)*z/(j+1);
        Sj1 = Sj + Cj1;
        err=std::abs(Cj1);
        Sj = Sj1;
        Cj=Cj1;
        j=j+1;
      }
      Q = 4.0/3.0 * Sj;
      tof = (eta*eta*eta*Q+4.0*lambda*eta)/2.0 + numberOfRevolutions*arma::datum::pi / pow(rho,1.5);
     
    } else { // We use Lancaster tof expresion
      double y=sqrt(rho);
      double g = param*z - lambda*E;
      double d = 0.0;
      if (E<0) {
        double l = acos(g);
        d=numberOfRevolutions*arma::datum::pi+l;
      } else {
        double f = y*(z-lambda*param);
        d=log(f+g);
      }
      tof = (param-lambda*z-d/y)/E;    
    }
  }

  std::cout << "pykep tof function result = " << tof << std::endl;
  
  // ----------------------------------------------------------------------
  // pykep call
  kep_toolbox::array3D r1 = {1000.0, 1500.0, 2500.0};
  kep_toolbox::array3D r2 = {1000.0, 2500.0, 2500.0};
  double callTOF = 800;
  
  kep_toolbox::lambert_problem lambertProblemObject(r1, r2, callTOF, 1.32712440018e20, 1);
  
  std::vector<kep_toolbox::array3D> v1 = lambertProblemObject.get_v1();
  std::vector<kep_toolbox::array3D> v2 = lambertProblemObject.get_v2();
  
  std::cout << std::endl << "--- PYKEP CALL ---" << std::endl << "v1" << std::endl;
  for(auto velo : v1){
    std::cout << velo << std::endl;
  }
  std::cout << "v2" << std::endl;
  for(auto velo : v2){
    std::cout << velo << std::endl;
  }

/*
  arma::Col<double>::fixed<3> depPos = {1000.0, 1500.0, 2500.0};
  arma::Col<double>::fixed<3> arrPos = {1000.0, 2500.0, 2500.0};

  std::vector<std::pair<arma::Col<double>::fixed<3>, arma::Col<double>::fixed<3>>> resVec = mant::itd::lambert(depPos, arrPos, callTOF);
*/
  return 0;
}