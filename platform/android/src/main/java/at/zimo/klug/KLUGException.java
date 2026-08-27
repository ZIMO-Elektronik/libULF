package at.zimo.klug;

/**
 * An exception to signal internal errors (any internal error actually)
 * 
 * This is mostly thrown from the underlying JNI.
 */
public class KLUGException extends RuntimeException {
    private final int errorCode;

    public KLUGException(int errorCode) {
        super("libklug error: " + errorCode);
        this.errorCode = errorCode;
    }

    public int getErrorCode() {
        return errorCode;
    }
}