using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Respawn : MonoBehaviour {

    public bool isDead;
    private float timer;
    public float spawnTime;
    public GameObject enemyMonster;

	// Use this for initialization
	void Start () {
        isDead = false;
        timer = 0;
	}
	
	// Update is called once per frame
	void Update () {
        if (isDead)
		{
            timer += Time.deltaTime;
        }

        if (timer >= spawnTime)
		{
			RespawnEnemy ();
        }
	}

	void RespawnEnemy(){
        //Debug.Log("Enemy trying to respawn");
        //Instantiate new GameObject with enemy prefab
        GameObject newEnemy = (GameObject)Instantiate (enemyMonster, transform.position, Quaternion.identity);
        //Set it active in the hierarchy so that it appears in scene
        newEnemy.SetActive(true);
        //Debug.Log(newEnemy);
        //Reset variables
        timer = 0;
        isDead = false;
    }

}
