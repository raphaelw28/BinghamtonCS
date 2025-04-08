class checklab3files
{
	public static void main(String[] args)
	{
		if( args.length != 2 )
		{
			System.out.println("checklab3files: \"file 1\" \"file 2\"");
			System.exit(0);
		}
		
		try
		{
			java.io.BufferedReader input1 = new java.io.BufferedReader(new java.io.InputStreamReader(new java.io.FileInputStream(args[0])));
			java.io.BufferedReader input2 = new java.io.BufferedReader(new java.io.InputStreamReader(new java.io.FileInputStream(args[1])));
			
			java.util.ArrayList<String> lines1 = new java.util.ArrayList<>();
			java.util.ArrayList<String> lines2 = new java.util.ArrayList<>();
			
			String inn;
			while( (inn = input1.readLine()) != null )
			{
				lines1.add(inn);
			}
			
			while( (inn = input2.readLine()) != null )
			{
				char[] origChars = inn.toCharArray();
				char[] reversedChars = new char[origChars.length];
				for( int i = 0; i < origChars.length; i++ )
				{
					reversedChars[i] = origChars[(origChars.length-1)-i];
				}
				inn = new String(reversedChars);
				lines2.add(inn);
			}
			
			if( lines1.size() != lines2.size() )
			{
				System.out.println("files do not match (size)");
				System.exit(0);
			}
			
			for( int i = 0; i < lines1.size(); i++ )
			{
				if( !lines1.get(i).equals(lines2.get(i)) )
				{
					System.out.println("files do not match (line " + (i+1) + ")");
					System.exit(0);
				}
			}
			
			System.out.println("files match");
			
			input1.close();
			input2.close();
		}
		catch(Exception e)
		{
			System.out.println(e.toString());
			System.exit(0);
		}
	}
}
