
public class Carte 
{
	private String numberCard;
	
	public Carte(String numberCard)
	{
		this.numberCard = numberCard;
	}
	
	public static boolean isNumber(String numeCarte)
	{
		boolean rep = false;
		
		if(numeCarte.length() == 16)
		{
			int i = 0;
			rep = true;
			
			while(i < numeCarte.length() && rep)
			{
				char number = numeCarte.charAt(i);
				boolean isNumber = java.lang.Character.isDigit(number);
				
				//verifie que le carectère est bien un nombre sinon on envoi rep false
				if(!isNumber)
				{
					rep = false;
				}
				i++;
			}
		}
	return rep;
	}
	
	public String toString()
	{
		return "carte:" + this.numberCard + ";";
	}
}