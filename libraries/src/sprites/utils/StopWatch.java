package sprites.utils;

public class StopWatch {

	private long currTime;
	private long lastTime;
	private long lapTime;
	private long startTime;
	
	
	public StopWatch(){
		reset();
	}
	
	public void reset(){
		currTime = lastTime = startTime = System.nanoTime();
		lapTime = 0;
	}
	
	public double getRunTime(){
		double rt = 1.0E-9 * (System.nanoTime() - startTime);
		return rt;
	}
	
	public double getElapsedTime(){
		currTime = System.nanoTime();
		lapTime = currTime - lastTime;
		lastTime = currTime;
		return 1.0E-9 * lapTime;
	}
	
}
