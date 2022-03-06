using Godot;
using System;
using System.Collections.Generic;
public class BaseEnemy : RigidBody2D
{
	public RigidBody2D Player;
	//protected RayCast2D SightLine;
	public int Health = 3;
	[Export] public float AttackPower = 1.0f;
	[Export] public float MovementSpeed = 50.0f;
	public bool PlayerFound = false;
	public bool InAttackRange = false;
	public Timer AttackCooldown;
	public bool AttackReady = true;
	[Export] public float TimeBetweenAttacks = 0.5f;
	[Signal] protected delegate void PlayerDamaged(float DamageTaken);
	public AnimatedSprite AnimSprite;
	public bool DeathAnimFinished = false;
	
	protected virtual void _on_EnemyBody_body_entered(Node body){	}
	protected virtual void _on_EnemyBody_body_exited(Node body) {   }
	protected void _on_HitBox_area_entered(Node area)
	{
		if(area.Name == "BulletArea2D")
		{
			Health -= 1;
		}
	}


	private void _on_HitBox_area_exited(Node area)
	{
		// Replace with function body.
		
	}




	protected virtual void AttackPlayer(){ }
	protected virtual void _on_Timer_timeout(){ }
	protected virtual void _on_DetectionArea_body_entered(object body)
	{
		
		if (body == Player)
		{
			PlayerFound = true;
		}
		//GD.Print(body.ToString());
	}
	protected virtual void _on_DetectionArea_body_exited(object body)
	{
		//GD.Print("Out of Detection Range");
		if(body == Player)
		{
			PlayerFound = false;
		}
		
	}

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{	
		AttackCooldown = GetNode<Timer>("AttackTimer");
		AnimSprite = GetNode<AnimatedSprite>("AnimatedSprite");
		Player = GetNode<RigidBody2D>("/root/World/Player/PlayerBody");

		
		Connect("PlayerDamaged", Player, "_on_EnemyBody_PlayerDamaged");
		//Connect("animation_finished", DeathMenu, "PlayerDeath");
		//GD.Print(Player.ToString());
	}

	// called every frame. 'delta' is the elapsed time since the previous frame.

	public override void _PhysicsProcess(float delta)
	{
		if (Health <= 0)
		{
			AnimSprite.Play("Death Anim");
			//death animation code
			//delay
			if (DeathAnimFinished)
			{
				QueueFree();
			}

		}
		
	}
	public void _on_AnimatedSprite_animation_finished()
	{
		if(AnimSprite.Animation == "Death Anim")
		{
			DeathAnimFinished = true;
		}
	}

}

