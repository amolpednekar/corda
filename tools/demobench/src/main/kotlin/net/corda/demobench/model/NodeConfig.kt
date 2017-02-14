package net.corda.demobench.model

import com.typesafe.config.*
import java.lang.String.join
import java.nio.file.Path
import net.corda.node.services.config.SSLConfiguration

class NodeConfig(
        baseDir: Path,
        legalName: String,
        artemisPort: Int,
        val nearestCity: String,
        val webPort: Int,
        val h2Port: Int,
        val extraServices: List<String>,
        val users: List<Map<String, Any>> = listOf(defaultUser)
) : NetworkMapConfig(legalName, artemisPort) {

    companion object {
        val renderOptions: ConfigRenderOptions = ConfigRenderOptions.defaults().setOriginComments(false)

        val defaultUser: Map<String, Any> = mapOf(
            "user" to "guest",
            "password" to "letmein",
            "permissions" to listOf(
                "StartFlow.net.corda.flows.CashFlow",
                "StartFlow.net.corda.flows.IssuerFlow\$IssuanceRequester"
            )
        )
    }

    val nodeDir: Path = baseDir.resolve(key)
    val explorerDir: Path = baseDir.resolve("$key-explorer")

    val ssl: SSLConfiguration = object : SSLConfiguration {
        override val certificatesDirectory: Path = nodeDir.resolve("certificates")
        override val trustStorePassword: String = "trustpass"
        override val keyStorePassword: String = "cordacadevpass"
    }

    var networkMap: NetworkMapConfig? = null

    var state: NodeState = NodeState.STARTING

    val isCashIssuer: Boolean = extraServices.any {
        it.startsWith("corda.issuer.")
    }

    fun isNetworkMap(): Boolean = networkMap == null

    /*
     * The configuration object depends upon the networkMap,
     * which is mutable.
     */
    fun toFileConfig(): Config = ConfigFactory.empty()
            .withValue("myLegalName", valueFor(legalName))
            .withValue("artemisAddress", addressValueFor(artemisPort))
            .withValue("nearestCity", valueFor(nearestCity))
            .withValue("extraAdvertisedServiceIds", valueFor(join(",", extraServices)))
            .withFallback(optional("networkMapService", networkMap, {
                c, n -> c.withValue("address", addressValueFor(n.artemisPort))
                    .withValue("legalName", valueFor(n.legalName))
            } ))
            .withValue("webAddress", addressValueFor(webPort))
            .withValue("rpcUsers", valueFor(users))
            .withValue("h2port", valueFor(h2Port))
            .withValue("useTestClock", valueFor(true))

    fun toText() = toFileConfig().root().render(renderOptions)

    fun moveTo(baseDir: Path) = NodeConfig(
        baseDir, legalName, artemisPort, nearestCity, webPort, h2Port, extraServices, users
    )
}

private fun <T> valueFor(any: T): ConfigValue? = ConfigValueFactory.fromAnyRef(any)

private fun addressValueFor(port: Int) = valueFor("localhost:$port")

private fun <T> optional(path: String, obj: T?, body: (Config, T) -> Config): Config {
    val config = ConfigFactory.empty()
    return if (obj == null) config else body(config, obj).atPath(path)
}
