package visualisation;

import org.armadillojava.Mat;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

/**
 *
 * @author Wolff
 */
public class ImageDisplay extends JPanel {

  private int width, height;
  private BufferedImage image = null;
  private Mat mat = null;
  private double min = 0;
  private double max = 0;

  public ImageDisplay(int width, int height) {
    this.width = width;
    this.height = height;
    image = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
  }

  @Override
  public void paintComponent(Graphics gfx) {
    super.paintComponent(gfx);
    Graphics2D g2d = (Graphics2D) gfx;
    synchronized (image) {
      g2d.drawImage(image, null, 0, 0);
    }
  }

  public void updateImageContents(Mat input) {
    mat = input;
    min = mat.min();
    max = mat.max();

    int xPart = width / input.n_cols;
    int xLeftOver = width % input.n_cols;
    int yPart = height / input.n_rows;
    int yLeftOver = height % input.n_rows;

    synchronized (image) {
      int lowerX = 0;
      int lowerY = 0;
      int upperX = 0;
      int upperY = 0;
      for (int y = 0; y < input.n_rows; y++) {
        upperY = lowerY + yPart;
        if (yLeftOver > 0) {
          yLeftOver--;
          upperY++;
        }
        for (int x = 0; x < input.n_cols; x++) {
          double val = mat.at(y, x);
          upperX = lowerX + xPart;
          if (xLeftOver > 0) {
            xLeftOver--;
            upperX++;
          }
          paintBlock(lowerX, upperX, lowerY, upperY, val);
          lowerX = upperX;
        }
        lowerX = 0;
        xLeftOver = width % input.n_cols;
        lowerY = upperY;
      }
    }

    SwingUtilities.invokeLater(new Runnable() {
      public void run() {
        repaint();
      }
    });

  }

  private int getARGBColor(double min, double max, double val) {
    float sat = 0.5f;
    float light = 0.5f;
    float hue = getHueFor(min, max, val);
    Color c = toRGB(hue, sat, light);

    return c.getRGB();
  }

  private float getHueFor(double min, double max, double val) {
    return (float) ((val - min) / (max - min)) * 240;
  }

  private void paintBlock(int lowerX, int upperX, int lowerY, int upperY, double val) {

    for (int x = lowerX; x < upperX; x++) {
      for (int y = lowerY; y < upperY; y++) {
        image.setRGB(x, y, getARGBColor(min, max, val));
      }
    }
  }
  
  /**
   * Convert HSL values to a RGB Color with a default alpha value of 1.
   *
   * @param h Hue is specified as degrees in the range 0 - 360.
   * @param s Saturation is specified as a percentage in the range 1 - 100.
   * @param l Luminance is specified as a percentage in the range 1 - 100.
   */
  public static Color toRGB(float h, float s, float l) {
    return toRGB(h, s, l, 1.0f);
  }

  /**
   * Convert HSL values to a RGB Color.
   *
   * @param h Hue is specified as degrees in the range 0 - 360.
   * @param s Saturation is specified as a percentage in the range 1 - 100.
   * @param l Luminance is specified as a percentage in the range 1 - 100.
   * @paran alpha the alpha value between 0 - 1
   */
  public static Color toRGB(float h, float s, float l, float alpha) {
    //  Formula needs all values between 0 - 1.

    h = h % 360.0f;
    h /= 360f;

    float q = 0;

    if (l < 0.5) {
      q = l * (1 + s);
    } else {
      q = (l + s) - (s * l);
    }

    float p = 2 * l - q;

    float r = Math.max(0, HueToRGB(p, q, h + (1.0f / 3.0f)));
    float g = Math.max(0, HueToRGB(p, q, h));
    float b = Math.max(0, HueToRGB(p, q, h - (1.0f / 3.0f)));

    return new Color(r, g, b, alpha);
  }

  private static float HueToRGB(float p, float q, float h) {
    if (h < 0) {
      h += 1;
    }

    if (h > 1) {
      h -= 1;
    }

    if (6 * h < 1) {
      return p + ((q - p) * 6 * h);
    }

    if (2 * h < 1) {
      return q;
    }

    if (3 * h < 2) {
      return p + ((q - p) * 6 * ((2.0f / 3.0f) - h));
    }

    return p;
  }
}
