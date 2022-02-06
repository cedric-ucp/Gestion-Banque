import java.sql.Date;

public class Transaction 
{
	private String num_compte_terminal;
	public final static String ID = "00000001";
	
	private String montant;
	
	
	public Transaction(String montant, String num_compte_terminal)
	{
		this.montant = montant;
		this.num_compte_terminal = num_compte_terminal;
	}
	
	public static boolean estMontant(String montant)
	{
		boolean rep = false;
		
		if(montant.length() <= 7)
		{
			boolean havePoint = false;
			rep = true;
			
			int i = 0;
			
			while( i < montant.length() && rep)
			{
				char number = montant.charAt(i);
				
				//les test pour savoir si le carectère est un point ou un nombre
				boolean isPoint = number == '.';
				boolean isNumber = java.lang.Character.isDigit(number);
				
				//verifie si le carectère est un nombre ou un point
				if(!isNumber && !isPoint)
				{
					rep = false;
					//System.out.println("carectère non définie");
				}
				
				//verifie si un seul point est entrer
				if(isPoint && havePoint)
				{
					rep = false;
					//System.out.println("Point en trop.");
				}
				
				//verifie que le montant n'aie pas plus loin que 9999.99
				if((i == 4) && !havePoint)
				{
					rep = false;
					//System.out.println("trop de chiffre");
				}
				
				//indique quand un carectère est un point
				if(isPoint)
				{
					havePoint = true;
				}
				
				i++;
			}
		}
	return rep;
	}
	
	public static boolean estNumCompte(String numCompte)
	{
		boolean rep = false;
		
		if(numCompte.length() == 8)
		{
			int i = 0;
			rep = true;
			
			while(i < numCompte.length() && rep)
			{
				char number = numCompte.charAt(i);
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
		String numCompte = "compte:" + this.num_compte_terminal + ";";
		String montant = "montant:" + this.montant + ";";
		
	return montant + numCompte;
	}
	
	}
