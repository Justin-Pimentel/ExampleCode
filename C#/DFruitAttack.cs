using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DFruitAttack : MonoBehaviour {
    public int attackDamage;
    private float lastAttackTime;
    public float attackDelay;

	// Use this for initialization
	void Start () {
        lastAttackTime = 0;
	}
	
    private void OnCollisionEnter2D(Collision2D collision)
    {
        //Debug.Log(collision.gameObject.name + " collided with DragonFruit!");
        if (collision.gameObject.name == "Player") {
            GameObject Player = collision.gameObject;
            //InvokeRepeating(Player.GetComponent<PlayerHealth>().TakeDamage(attackDamage), 0.1f, attackDelay);
            if (Time.time > lastAttackTime + attackDelay)
            {
                Player.GetComponent<PlayerHealth>().TakeDamage(attackDamage);
            }
        }
    }
}
