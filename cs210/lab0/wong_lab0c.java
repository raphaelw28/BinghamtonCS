class wong_lab0c
{
	public static void main(String[] args)
	{
		System.out.println("yo, i am here, and below are the command line arguments you gave me");
		for(int i = 0; i < args.length; i++ )
		{
			System.out.println("\t arg[" + i +"] = " + args[i]);
		}
		
		for( int i = 0; i < args.length; i++ )
		{
		try
		{
			int intValue = Integer.parseInt(args[i]);
			System.out.println("\t arg[" + i + "] as an int = " + intValue);
		}
		catch(NumberFormatException e)
		{
			System.out.println("\t arg[" + i + "] as an int " + args[i] + " is not a valid integer");
		}
	   }
	}
}
