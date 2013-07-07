/*
  Part of the Sprites for Processing library 
  	http://sprites4processing.lagers.org.uk
	http://code.google.com/p/sprites4processing/svn/trunk

  Copyright (c) 2009 Peter Lager

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
 */

package sprites;

public class SMessenger implements SConstants{
	public static void message(Integer id, Object obj, Object[] info){
		// Display S4P messages if required
		if(S4P.messages){
			switch(id){
			case MISSING:
				missingEventHandler(obj, info);
				break;
			case NONEXISTANT:
				nonexistantEventHandler(obj, info);
				break;
			}
		}
		// Display all runtime errors
		switch(id){
		case EXCP_IN_HANDLER:
			eventHandlerFailed(obj, info);
			break;
		}
	}
	
	/**
	 * 
	 * @param handler
	 * @param info
	 */
	private static void eventHandlerFailed(Object handler, Object[] info) {
		String className = handler.getClass().getSimpleName();
		String methodName = (String) info[0];
		Exception e = (Exception) info[1];
		Throwable t = e.getCause();
		StackTraceElement[] calls = t.getStackTrace();
		StringBuilder output = new StringBuilder();
		output.append("#############  EXCEPTION IN EVENT HANDLER  #############");
		output.append("\nAn error occured during execution of the eventhandler:");
		output.append("\nCLASS: "+className+"   METHOD: "+methodName);
		output.append("\n\tCaused by " + t.toString());
		if(calls.length > 0)
			output.append("\n\t"+ calls[0].toString());
		System.out.println(output);
		System.out.println("########################################################\n");
	}

	/**
	 * 
	 * @param obj1 the object generating the method
	 * @param obj2 the method name
	 * @param obj3 parameter types (Class[])
	 */
	@SuppressWarnings("unchecked")
	private static void missingEventHandler(Object caller, Object[] info) {
		String className = caller.getClass().getSimpleName();
		String methodName = (String) info[0];
		String pname;
		StringBuilder output = new StringBuilder();
		
		output.append("You might want to add a method to handle " + className + " events syntax is\n");
		output.append("void " + methodName + "(");
		if(info != null && info.length > 1){
			Class[] parameters = (Class[])(info[1]);
			if(parameters == null)
				parameters = new Class[0];
			for(int i = 0; i < parameters.length; i++){
				pname = (parameters[i]).getSimpleName();
				output.append(pname + " " + pname.substring(0).toLowerCase());
				if(parameters.length > 1)
					output.append(i);
				if(i < parameters.length - 1)
					output.append(", ");
			}
		}
		output.append(") { /* code */ }\n");
		System.out.println(output.toString());
	}

	/**
	 * 
	 * @param obj1 the object generating the method
	 * @param obj2 the method name
	 * @param obj3 parameter types (Class[])
	 */
	@SuppressWarnings("unchecked")
	private static void nonexistantEventHandler(Object handler, Object[] info) {
		String className = handler.getClass().getSimpleName();
		String methodName = (String) info[0];
		String pname;
		StringBuilder output = new StringBuilder();
		
		output.append("The "+className+" class does not have this method \n");
		output.append("\tvoid " + methodName + "(");
		if(info != null && info.length > 1){
			Class[] parameters = (Class[])(info[1]);
			if(parameters == null)
				parameters = new Class[0];
			for(int i = 0; i < parameters.length; i++){
				pname = (parameters[i]).getSimpleName();
				output.append(pname + " " + pname.substring(1).toLowerCase());
				if(parameters.length > 1)
					output.append(i);
				if(i < parameters.length - 1)
					output.append(", ");
			}
		}
		output.append(") { /* code */ }\n");
		System.out.println(output.toString());
	}

}
