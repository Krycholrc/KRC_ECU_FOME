package com.rusefi.newparse.outputs;

import com.rusefi.newparse.ParseState;
import com.rusefi.newparse.layout.StructLayout;
import com.rusefi.newparse.layout.StructNamePrefixer;
import com.rusefi.newparse.parsing.Struct;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

/**
 * As of Nov 2022 this implementation is not used in prod :(
 */
public class OutputChannelWriter {
    private final PrintStream ps;
    private final PrintStream psDatalog;

    public OutputChannelWriter(String outputFile, String datalogOutputFile) throws FileNotFoundException {
        this.ps = new PrintStreamAlwaysUnix(new FileOutputStream(outputFile));
        this.psDatalog = new PrintStreamAlwaysUnix(new FileOutputStream(datalogOutputFile));
    }

    public OutputChannelWriter(PrintStream ps, PrintStream psDatalog) {
        this.ps = ps;
        this.psDatalog = psDatalog;
    }

    private int cumulativeSize = 0;

    public void writeOutputChannels(ParseState parser, String namePrefix) {
        // Assume the last struct is the one we want...
        Struct s = parser.getStructs().get(parser.getStructs().size() - 1);

        StructLayout sl = new StructLayout(0, "root", s);

        StructNamePrefixer prefixer = new StructNamePrefixer('_');

        if (namePrefix != null) {
            prefixer.push(namePrefix.replace(" ", "_"));
        }

        OutputChannelVisitor v = new OutputChannelVisitor(namePrefix);
        sl.visit(v, ps, prefixer, cumulativeSize, new int[0]);

        DatalogVisitor dlv = new DatalogVisitor(namePrefix);
        sl.visit(dlv, psDatalog, prefixer, cumulativeSize, new int[0]);

        cumulativeSize += sl.getSize();
        ps.println("; total TS size = " + cumulativeSize);
    }

    public int getSize() {
        return cumulativeSize;
    }
}
