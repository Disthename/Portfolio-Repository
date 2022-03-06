using Godot;
using System;

public class BossEnemy : ProjectileEnemy
{
	PackedScene MeleeEnemy;
	Area2D SpawnArea;
	RandomNumberGenerator rng = new RandomNumberGenerator();
	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		//SpawnArea = GetNode<Area2D>("SpawnArea");
		TimeBetweenAttacks = 0.1f;
		
		base._Ready();
		Health = 40;
		MeleeEnemy = GD.Load<PackedScene>("res://Scenes/MeleeEnemy.tscn");
	}
	new protected void _on_HitBox_area_entered(Node area)
	{
		if (area.Name == "BulletArea2D")
		{
			Health -= 1;
			//CallDeferred("BossSpawningFunction");
			 

		}
	}
	//public void BossSpawningFunction()
	//	{
	//	GD.Print("Enemy Spawned");
	//	Vector2 centerPos = SpawnArea.Position;
	//	Node spawn = MeleeEnemy.Instance();
	//	Vector2 positionInArea = new Vector2((rng.Randi() % 5) - (5 / 2) + centerPos.x, (rng.Randi() % 5) - (5 / 2) + centerPos.y);
	//	spawn.GetNode<RigidBody2D>("EnemyBody").Position = positionInArea;
	//	this.AddChild(spawn);

	//}
	//  // Called every frame. 'delta' is the elapsed time since the previous frame.
	//  public override void _Process(float delta)
	//  {
	//      
	//  }
}
