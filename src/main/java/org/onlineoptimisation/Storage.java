package org.onlineoptimisation;

import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;

public class Storage {

  @SafeVarargs
  public static void CSV(Path filepath, List<Object>... inputs) throws IOException {
    PrintWriter writer = new PrintWriter(Files.newBufferedWriter(filepath, StandardCharsets.UTF_8));
    
    if(inputs.length > 0) {
      for(int n = 0; n < inputs[0].size(); n++) {
        writer.print(n + 1);
        for(List<Object> values : inputs) {
          writer.print(" " + values.get(n));
        }
        writer.println();
      }
    }

    writer.close();
  }
  
}
