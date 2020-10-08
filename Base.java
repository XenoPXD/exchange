package impl;

import java.util.Arrays;

/**
 * @author Xéno_PXD
 * 08/10/2020
 */
public class Base {

	private int base;
	private int[] bitVector;
	private boolean isBalanced = false;
	
	public Base() {}

	// https://en.wikipedia.org/wiki/Bitwise_operation
	private static int shiftRightLogical(Base b1, Base b2) {
		
		int dec = 0;
		int b1NbrBits = b1.getBitVector().length;		
		int b2Length = Base.toDec(b2.getBitVector(), b2.getBase());
		if (b2Length > b1NbrBits) b2Length = b1NbrBits;
		
		int[] oldBitVector = b1.getBitVector();
		int[] newBitVector = new int[oldBitVector.length];
		for (int i = 0; i < b2Length; i++) {
			newBitVector[i] = 0;
//			String info = String.format("newBitVector[%d]=%d", 0, newBitVector[0]);
//			System.out.println(info);
		}
		
		for (int j = b2Length; j < oldBitVector.length; j++) {
//			String info = String.format("oldBitVector[%d]=%d", j-b2Length, oldBitVector[j-b2Length]);
//			System.out.println(info);
			newBitVector[j] = oldBitVector[j-b2Length];
//			info = String.format("newBitVector[%d]=%d", j, newBitVector[j]);
//			System.out.println(info);
		}
		
		dec = Base.toDec(newBitVector, b1.getBase());
		
		
		return dec;
	}

	private static int shiftLeftLogical (Base b1, Base b2) {
		
		int b2Length = Base.toDec(b2.getBitVector(), b2.getBase());
		
		int[] oldBitVector = b1.getBitVector();
		int[] newBitVector = new int[oldBitVector.length+b2Length];
		for (int i = 0; i < oldBitVector.length; i++) {
			newBitVector[i] = oldBitVector[i];
//			String info = String.format("newBitVector[%d]=%d", i, newBitVector[i]);
//			System.out.println(info);
		}
		for (int i = oldBitVector.length; i < oldBitVector.length+b2Length; i++) {
			newBitVector[i] = 0;
//			String info = String.format("newBitVector[%d]=%d", i, newBitVector[i]);
//			System.out.println(info);
		}
		
		int dec3 = Base.toDec(newBitVector, b1.getBase());
		
		return dec3;
	}

	private static int computeOperator(Operator operator, Base b1, Base b2) {
		int dec = 0;
		if (operator.equals(Operator.ADDITION)) {
			dec = b1.toDec()+b2.toDec();
		} else if (operator.equals(Operator.SUBTRACTION)) {
			dec = b1.toDec()-b2.toDec();
		} else if (operator.equals(Operator.MULTIPLICATION)) {
			dec = b1.toDec()*b2.toDec();
		} else if (operator.equals(Operator.SHIFT_RIGHT_LOGICAL)) {
			dec = shiftRightLogical(b1, b2);
		} else if (operator.equals(Operator.SHIFT_LEFT_LOGICAL)) {
			dec = shiftLeftLogical(b1, b2);
		}
		return dec;
	}
	
	public static Base operator(Operator operator, Base b1, Base b2) 
			throws IncorrectBaseBalancedException, 
			InconsistencyBaseException, 
			InconsistencyBalancedException, 
			OperatorMandatoryException {
		
		if (operator == null) {
			throw new OperatorMandatoryException("Operator is null.");
		} else if (b1.getBase()!=b2.getBase()) {
			String errorMessage = String.format("Inconsistency base1 = [%d] and base2 = [%d]", b1.getBase(), b2.getBase());
			throw new InconsistencyBaseException(errorMessage);
		} else if (b1.isBalanced()!=b2.isBalanced()) {
			String errorMessage = String.format("Inconsistency balanced1 = [%s] and balanced2 = [%s]", b1.isBalanced(), b2.isBalanced());
			throw new InconsistencyBalancedException(errorMessage);
		}
		int dec = computeOperator(operator, b1, b2);
		return Base.create(dec, b1.getBase(), b1.isBalanced);
	}
	
	public int toDec() {
		int t = 0;
		int[] bitVectoreTmp = Base.getReverse(this.bitVector);
		int length = bitVectoreTmp.length;
		for (int i = length; i > 0; i--) {
			int u = (int) Math.pow(this.base, i - 1);
			int w = bitVectoreTmp[i - 1] * u;
			t += w;
		}
		return t;
	}
	
	public static int toDec(int[] inBitVector, int inBase) {
		int[] bitVectoreTmp = Base.getReverse(inBitVector);
		int t = 0;
		int length = bitVectoreTmp.length;
		for (int i = length; i > 0; i--) {
			int u = (int) Math.pow(inBase, i - 1);
			int w = bitVectoreTmp[i - 1] * u;
			t += w;
		}
		return t;
	}
	
	public Base toBase(int inBase, boolean isBalanced) throws IncorrectBaseBalancedException {
		int inputNum = this.toDec();
		return Base.create(inputNum, inBase, isBalanced);
	}
	
	public static Base create(int[] inBitVector, int inBase, boolean isBalanced) throws IncorrectBaseBalancedException {
		int dec = Base.toDec(inBitVector, inBase);
		return Base.create(dec, inBase, isBalanced);
	}

	public static Base create(int inputNum, int inBase, boolean isBalanced) throws IncorrectBaseBalancedException {
		
		Base outBase = new Base();
		if (isBalanced) {
			outBase = createBalanced(inputNum, inBase);
		} else {
			outBase = createStandard(inputNum, inBase);
		}
		
		return outBase;
	}

	public static Base createBalanced(int inputNum, int inBase) throws IncorrectBaseBalancedException {
		
		if (inBase % 2 == 0) {
			throw new IncorrectBaseBalancedException("Incorrect base : [" + inBase + "] for a base balanced." );
		}
		
		if (inputNum == 0) {
			Base outBase = new Base();
			outBase.setBitVector(new int[] {0});
			outBase.setBase(inBase);
			outBase.setBalanced(true);
			return outBase;
		}
		
		Base outBase = new Base();
		int n = Math.abs(inputNum);
		int[] bitVector = {};
		int i = 0;
		int p = inBase / 2;
		while (n > 0) {
			int rem = n % inBase;
			n = n / inBase;
			if (rem > p) {
				rem -= inBase;
				n++;
			}
			bitVector = Arrays.copyOf(bitVector, bitVector.length + 1);
			if (inputNum < 0) rem = rem*-1;
			bitVector[i] = rem ;
			i++;
		}
		
		outBase.setBitVector(Base.getReverse(bitVector));
		outBase.setBase(inBase);
		outBase.setBalanced(true);
		
		return outBase;
	}

	public static Base createStandard(int inputNum, int inBase) {
		
		if (inputNum == 0) {
			Base outBase = new Base();
			outBase.setBitVector(new int[] {0});
			outBase.setBase(inBase);
			outBase.setBalanced(false);
			return outBase;
		}
		
		Base outBase = new Base();
		int[] bitVector = {};
		int i = 0;
		while (inputNum > 0) {
			bitVector = Arrays.copyOf(bitVector, bitVector.length+1);
			bitVector[i] = inputNum % inBase;
			i++;
			inputNum /= inBase;
		}
		outBase.setBitVector(Base.getReverse(bitVector));
		outBase.setBase(inBase);
		outBase.setBalanced(false);
		
		return outBase;
	}

	public int getBase() {
		return base;
	}
	public void setBase(int base) {
		this.base = base;
	}
	public int[] getBitVector() {
		return bitVector;
	}
	public void setBitVector(int[] bitVector) {
		this.bitVector = bitVector;
	}
	public boolean isBalanced() {
		return isBalanced;
	}
	public void setBalanced(boolean isBalanced) {
		this.isBalanced = isBalanced;
	}

	/**
	 * toString
	 * @see https://unicode-table.com/fr/blocks/superscripts-and-subscripts/
	 */
	public String toString() {
		String vector = "";
		vector = this.toDec()+"\u2081\u2080={";
		for (int i = 0; i < this.bitVector.length; i++) {
			vector += this.bitVector[i];
			if (i != this.bitVector.length-1) {
				vector += ",";
			}
		}
		vector += "}";
		String indiceBase = "";
		String strBase = String.valueOf(base);
		for (int i = 0; i < strBase.length(); i++) {
			String unicode = "208" + strBase.substring(i, i+1);
			indiceBase += String.valueOf(Character.toChars(Integer.parseInt(unicode, 16)));
		}
		vector += indiceBase;
		String e = isBalanced() ? "\u208Dbal\u208E" : "";
		vector += e;
		return vector;
	}
	
	public static int[] getReverse(int[] inArray) {
		int n = inArray.length;
		int[] outArray = new int[n]; 
        int j = n; 
        for (int i = 0; i < n; i++) { 
        	outArray[j - 1] = inArray[i]; 
            j = j - 1; 
        }
        return outArray;
	}
	
	public static int[] tetaToBitVector(String value, String teta) {
		int[] bitVecteur = new int[value.length()];
		for (int i = 0; i < value.length(); i++) {
			if (value.substring(i,  i+1).equals(teta)) {
				bitVecteur[i] = -1;
			} else {
				bitVecteur[i] = Integer.parseInt(value.substring(i,  i+1));
			}
		}
		return bitVecteur;
	}
	
	public static String bitVectorToTeta(int[] bigVector, String teta) {
		String out = "";
		for (int i = 0; i < bigVector.length; i++) {
			if (bigVector[i] == -1) {
				out += teta;
			} else {
				out += String.valueOf(bigVector[i]);
			}
		}
		return out;
	}
	
	public String toTeta(String teta) {
		String out = "";
		for (int i = 0; i < this.getBitVector().length; i++) {
			if (this.getBitVector()[i] == -1) {
				out += teta;
			} else {
				out += String.valueOf(this.getBitVector()[i]);
			}
		}
		return out;
	}
	
}

@SuppressWarnings("serial")
class IncorrectBaseBalancedException extends Exception { 
    public IncorrectBaseBalancedException(String errorMessage) {
        super(errorMessage);
    }
}

@SuppressWarnings("serial")
class InconsistencyBaseException extends Exception { 
    public InconsistencyBaseException(String errorMessage) {
        super(errorMessage);
    }
}

@SuppressWarnings("serial")
class InconsistencyBalancedException extends Exception { 
    public InconsistencyBalancedException(String errorMessage) {
        super(errorMessage);
    }
}

@SuppressWarnings("serial")
class OperatorMandatoryException extends Exception { 
    public OperatorMandatoryException(String errorMessage) {
        super(errorMessage);
    }
}

@SuppressWarnings("serial")
class OperatorIndefinedException extends Exception { 
    public OperatorIndefinedException(String errorMessage) {
        super(errorMessage);
    }
}



// https://fr.qaz.wiki/wiki/Balanced_ternary
enum  Operator {
	ADDITION ("+"),
	SUBTRACTION ("-"),
	MULTIPLICATION ("*"),
	SHIFT_RIGHT_LOGICAL (">>"),
	SHIFT_RIGHT_ARITHMETIC (">>>"),
	SHIFT_LEFT_ARITHMETIC ("<<<"),
	SHIFT_LEFT_LOGICAL  ("<<");

	private String code;
	
	Operator(String string) {
		this.code = string;
	}
	
	public static Operator getOperator(String code) {
		for (Operator operator : Operator.values()) {
			if (operator.getCode().equals(code)) {
				return operator;
			}
		}
		return null;
	}
	
	public String getCode() {
		return code;
	}

	public void setCode(String code) {
		this.code = code;
	}
	   
}

