//marla aguilar
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;

class HiloEjecucion {
    private final String nombre;
    private int tiempoEjecucion;

    public HiloEjecucion(String nombre, int tiempoEjecucion) {
        this.nombre = nombre;
        this.tiempoEjecucion = tiempoEjecucion;
    }

    public String getNombre() {
        return nombre;
    }

    public int getTiempoEjecucion() {
        return tiempoEjecucion;
    }

    public void decrementarTiempoEjecucion(int cantidad) {
        tiempoEjecucion -= cantidad;
        if (tiempoEjecucion < 0) {
            tiempoEjecucion = 0;
        }
    }
}

public class SimulacionSJN extends JFrame {
    private static final int MIN_NUM_HILOS = 5;
    private static final int MAX_NUM_HILOS = 10;
    private static final int MAX_LABEL_WIDTH = 200;

    private JLabel[] labels;
    private HiloEjecucion[] hilos;
    private int hiloActual;

    public SimulacionSJN() {
        setTitle("Simulación SJN");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new GridLayout(1, 1));

        int numHilos = (int) (Math.random() * (MAX_NUM_HILOS - MIN_NUM_HILOS + 1)) + MIN_NUM_HILOS;

        labels = new JLabel[numHilos];
        hilos = new HiloEjecucion[numHilos];
        hiloActual = 0;

        for (int i = 0; i < numHilos; i++) {
            int tiempo = (int) (Math.random() * 5000) + 1000;
            hilos[i] = new HiloEjecucion("Hilo" + (i + 1), tiempo);

            labels[i] = new JLabel();
            labels[i].setOpaque(true);
            labels[i].setBackground(getRandomColor());
        }

        Arrays.sort(hilos, (h1, h2) -> Integer.compare(h1.getTiempoEjecucion(), h2.getTiempoEjecucion()));

        add(labels[hiloActual]);

        setSize(300, 300);
        setLocationRelativeTo(null);
        setVisible(true);

        Timer timer = new Timer(1000, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ejecutarSJN(e);
            }
        });
        timer.start();
    }

    private void ejecutarSJN(ActionEvent e) {
        int reduccion = (int) (Math.random() * 40) + 1;
        labels[hiloActual].setSize(Math.max(labels[hiloActual].getWidth() - reduccion, 0), labels[hiloActual].getHeight());
        hilos[hiloActual].decrementarTiempoEjecucion(1000);

        if (labels[hiloActual].getWidth() <= 0) {
            hiloActual++;

            if (hiloActual < labels.length) {
                remove(labels[hiloActual - 1]);
                Arrays.sort(hilos, (h1, h2) -> Integer.compare(h1.getTiempoEjecucion(), h2.getTiempoEjecucion()));
                add(labels[hiloActual]);

                int newWidth = Math.min(labels[hiloActual].getWidth(), MAX_LABEL_WIDTH);
                labels[hiloActual].setSize(newWidth, labels[hiloActual].getHeight());

                revalidate();
                repaint();
            } else {
                ((Timer) e.getSource()).stop();
            }
        }
    }

    private Color getRandomColor() {
        return new Color((int) (Math.random() * 256), (int) (Math.random() * 256), (int) (Math.random() * 256));
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new SimulacionSJN();
            }
        });
    }
}
