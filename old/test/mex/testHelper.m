classdef testHelper
   methods(Static)
      function CHECK(isTrue)
        if ~isTrue
          exit 1
        end
      end
      
      function isTrue = hasSameElements(actualElements, expectedElements)
        if iscell(expectedElements)
          
        else
          for n = 1:size(expectedElements, 2)
          [~, rowIndex] = ismember(expectedElements(n, :), actualElements, 'rows');
          if rowIndex == 0
            isTrue = 0;
            return
          end
          
          actualElements(rowIndex, :) = [];
        end
      end
      
      function isTrue = hasSameDistribution(actualDistribution, expectedDistribution)
        if isempty(actualDistribution) ~= isempty(expectedDistribution)
          isTrue = 0;
          return
        elseif isempty(actualDistribution) && isempty(expectedDistribution)
          isTrue = 1;
          return
        end

        % Uses a Kolmogorov-Smirnov test with significance level 95%.
        isTrue = (max(abs(actualDistribution - expectedDistribution)) * sqrt(size(expectedDistribution, 1) / 2.0) <= sqrt(log(2.0 / 0.9) / 2.0));
      end
      
      function isTrue = isUniformDistributed(data, lowerBound, upperBound)
        if isempty(data)
          isTrue = 0;
          return
        end
      
        uniformDistribtion = lowerBound + rand(size(data, 1), 1) .* (upperBound - lowerBound);

        isTrue = (testHelper.hasSameDistribution(cumsum(hist(data, 10)) / size(data, 1), cumsum(hist(uniformDistribtion, 10) / size(data, 1))));
      end
      
      function isTrue = isNormalDistributed(data, standardDeviation)
        if isempty(data)
          isTrue = 0;
          return
        end

        normalDistribution = randn(size(data, 1), 1) .* standardDeviation;

        isTrue = (testHelper.hasSameDistribution(cumsum(hist(min(max(data, -100), 100), 10)) / size(data, 1), cumsum(hist(min(max(normalDistribution, -100), 100), 10) / size(data, 1))));
      end
    end
end