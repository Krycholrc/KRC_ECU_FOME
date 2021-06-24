package com.rusefi.newparse.layout;

import com.rusefi.newparse.parsing.*;

import java.io.PrintStream;

public class ArrayIterateStructLayout extends ArrayLayout {
    public ArrayIterateStructLayout(StructField prototype, int length) {
        super(prototype, length);
    }

    private void emitOne(PrintStream ps, StructNamePrefixer prefixer, int offset, int idx) {
        // Set element's position within the array
        int offsetAdd = offset + this.prototypeLayout.getSize() * idx;

        // Put a 1-based index on the end of the name to distinguish in TS
        prefixer.setIndex(idx);
        this.prototypeLayout.writeTunerstudioLayout(ps, prefixer, offsetAdd);
        prefixer.clearIndex();
    }

    @Override
    protected void writeTunerstudioLayout(PrintStream ps, StructNamePrefixer prefixer, int offsetAdd) {
        // Time to iterate: emit one scalar per array element, with the name modified accordingly

        for (int i = 0; i < this.length; i++) {
            emitOne(ps, prefixer, this.offset + offsetAdd, i);
        }
    }

    // C layout is the same if iterated or not, use default implementation
}
